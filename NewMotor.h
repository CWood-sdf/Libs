#include <vector>
using namespace std;
#define nmdefaultRot rotationUnits::rev
#define vfor(it, v)    \
	for(auto it : v)
#define vpfor(it, v)   \
	for(auto* it : v)
#define vrfor(it, v)   \
for (auto& it : v)
#define vrep(it, v) \
	for(int it = 0; it < v.size(); it++)
#define prop(obj, name) \
	obj.name
#ifndef CHAIN
#define CHAIN return *this
#endif

namespace vex { 
	class motor; 
	enum class directionType;
	enum class percentUnits;
}
using namespace vex;
class Pto {
	int index = 0;
public:
	Pto(int i) : index(i) {}
	operator int(){
		return index;
	}
};
class NewMotor {
	int size = 0;
	static inline map<motor*, vector<NewMotor*>> ptoMotors;
	typedef motor motor_type;
	typedef NewMotor& chain_method;
	vector<double> lastVoltCmd = {};
	vector<motor_type*> m = vector<motor_type*>();
	vector<tuple<bool, vector<pneumatics*>, vector<int>>> pto = {};
	//might as well overallocate, this is just temporary anyway
	vector<bool> allowedMotors = vector<bool>(10, true);
	template<typename... args>
	void addMotor(args*... a) {
		m = { a... };
		size = m.size();
		lastVoltCmd = vector<double>(size, 0.0);
	}
	void setPtoAllowed(){
		//Fill allowedMotors with true
		allowedMotors = vector<bool>(m.size(), true);
		//Loop through pto and if first pneumatic matches the bool, fill allowedMotors indexes with false in allowedMotors
		for(auto& t : pto){
			if(get<0>(t) == get<1>(t)[0]->value()){
				for(int i : get<2>(t)){
					allowedMotors[i] = false;
				}
			}
		}
	}
	void reinvokeLast(){
		setPtoAllowed();
		for(int i = 0; i < size; i++){
			if(allowedMotors[i]){
				m[i]->spin(directionType::fwd, lastVoltCmd[i], percentUnits::pct);
			}
		}
	}
public:
	template<typename... args>
	NewMotor(args*... a) {
		addMotor(a...);
	}
	template<typename... args>
	NewMotor(args&... a) {
		addMotor(&a...);
	}
	NewMotor() {
		m = vector<motor_type*>();
		lastVoltCmd = vector<double>();
	}
	NewMotor(const NewMotor&) = default;
	/**
	 * @brief Adds a pto on the given motors
	 * 
	 * @param p the pneumatics
	 * @param motors the motors to run
	 * @param desiredState runs the motors if the pneumatics are in this state
	 * @return int the index of the PTO, store it in a Pto object
	 */
	int addPto(pneumatics& p, vector<int> motors, bool desiredState = true) {
		for(int i : motors){
			if(i >= m.size()){
				cerr << "NewMotor::addPto: Motor index out of range" << endl;
				return -1;
			}
		}
		//Loop through motors and add all motors to ptoMotors
		for(int i : motors){
			//Check if ptoMotors does not contain the motor
			if(ptoMotors.count(m[i]) == 0){
				//Add the motor to ptoMotors
				ptoMotors[m[i]] = vector<NewMotor*>();
			}
			ptoMotors[m[i]].push_back(this);
		}
		pto.push_back(make_tuple(desiredState, vector<pneumatics*>{&p}, motors));
		return pto.size() - 1;
	}
	// /**
	//  * @brief Adds a pto on the given motors
	//  * 
	//  * @param p the pneumatics
	//  * @param motors the motors to run
	//  * @param desiredState runs the motors if the pneumatics are in this state
	//  * @return int the index of the PTO, store it in a Pto object
	//  */
	// int addPto(vector<pneumatics*> p, vector<int> motors, bool desiredState = true) {
	// 	for(int i : motors){
	// 		if(i >= m.size()){
	// 			cerr << "NewMotor::addPto: Motor index out of range" << endl;
	// 			return -1;
	// 		}
	// 	}
	// 	//Loop through motors and add all pointers to the map
	// 	for(int i : motors){
	// 		if(ptoMotors.count(m[i]) == 0){
	// 			ptoMotors[m[i]] = vector<NewMotor*>();
	// 		}
	// 		ptoMotors[m[i]].push_back(this);
	// 	}
	// 	pto.push_back(make_tuple(desiredState, p, motors));
	// 	return pto.size() - 1;
	// }
	int indexOf(motor_type* m) {
		for(int i = 0; i < size; i++){
			if(m == this->m[i]){
				return i;
			}
		}
		return -1;
	}
	// /**
	//  * @brief Adds a pto on the given motors
	//  * 
	//  * @param p the pneumatics
	//  * @param motors the motors to run
	//  * @param desiredState runs the motors if the pneumatics are in this state
	//  * @return int the index of the PTO, store it in a Pto object
	//  */
	// int addPto(vector<pneumatics*> p, vector<motor*> motors, bool desiredState = true){
	// 	vector<int> motorIndexes = {};
	// 	for(motor* mot : motors){
	// 		int index = indexOf(mot);
	// 		if(index < 0){
	// 			cerr << "NewMotor::addPto: Motor not found" << endl;
	// 			return -1;
	// 		}
	// 		motorIndexes.push_back(index);
	// 	}
	// 	return addPto(p, motorIndexes, desiredState);
	// }
	/**
	 * @brief Adds a pto on the given motors
	 * 
	 * @param p the pneumatics
	 * @param motors the motors to run
	 * @param desiredState runs the motors if the pneumatics are in this state
	 * @return int the index of the PTO, store it in a Pto object
	 */
	int addPto(pneumatics& p, vector<motor*> motors, bool desiredState = true){
		vector<int> motorIndexes = {};
		for(motor* mot : motors){
			int index = indexOf(mot);
			if(index < 0){
				cerr << "NewMotor::addPto: Motor not found" << endl;
				return -1;
			}
			motorIndexes.push_back(index);
		}
		return addPto(p, motorIndexes, desiredState);
	}
	chain_method setPtoDrive(int ptoIndex){
		if(ptoIndex >= pto.size() || ptoIndex < 0){
			cerr << "NewMotor::setPtoDrive: PTO index out of range" << endl;
			CHAIN;
		}
		//Loop through desired pto and set the pneumatics to the desired state
		for(auto& p : get<1>(pto[ptoIndex])){
			p->set(get<0>(pto[ptoIndex]));
		}
		reinvokeLast();
		CHAIN;
	}
	chain_method setPtoRelease(int ptoIndex){
		if(ptoIndex >= pto.size() || ptoIndex < 0){
			cerr << "NewMotor::setPtoRelease: PTO index out of range" << endl;
			CHAIN;
		}
		//Loop through desired pto and set the pneumatics to the opposite of the desired state
		for(auto& p : get<1>(pto[ptoIndex])){
			p->set(!get<0>(pto[ptoIndex]));
		}
		//Get the motors that are not allowed to be driven
		vector<int> notAllowed = get<2>(pto[ptoIndex]);	
		//Loop through notAllowed and set the map pointers to reinvoke
		for(int i : notAllowed){
			//Loop through the pointers in the map
			for(auto* n : ptoMotors[m[i]]){
				//If the pointer is not this, reinvoke
				if(n != this){
					n->reinvokeLast();
					//Don't want a multiple reinvoke
					break;
				}
			}
		}
		CHAIN;
	}
	motor_type& operator[](int n) {
		return *m[n];
	}
	
	//Stop all the motors, but only the ones that are false in the exceptions list
	void stop(brakeType brak, vector<bool> exceptions) {
		int i = 0;
		for (auto n : m) {
			if (i >= exceptions.size() || exceptions[i]) {
				n->stop(brak);
			}
			i++;
		}
		lastVoltCmd = vector<double>(size, 0.0);
	}
	
	void stop() {
		for (auto n : m) {
			n->stop();
		}
		lastVoltCmd = vector<double>(size, 0.0);
	}
	//spin fwd
	chain_method spin(directionType dir, int velocity, percentUnits v = pct) {
		setPtoAllowed();
		int i = 0;
		for (auto n : m) {
			if(allowedMotors[i]){
				n->spin(dir, velocity, v);
			}
		}
		lastVoltCmd = vector<double>(size, velocity * (v == pct ? 1.0 : 120.0 / 100.0) * (dir == fwd ? 1.0 : -1.0));
		return *this;
	}
	//spin a specific motor
	chain_method spin(directionType dir, int velocity, int n) {
		setPtoAllowed();
		if(allowedMotors[n]){
			m[n]->spin(dir, velocity, pct);
		}
		lastVoltCmd[n] = velocity * (pct == pct ? 1.0 : 120.0 / 100.0) * (dir == fwd ? 1.0 : -1.0);
		return *this;
	}
	//spinVolt the motors, but only the ones that are false in the exceptions list
	chain_method spinVolt(directionType dir, int velocity) {
		setPtoAllowed();
		int i = 0;
		for (auto n : m) {
			if (allowedMotors[i]) {
				n->spin(dir, velocity * 0.12, volt);
			}
			i++;
		}
		lastVoltCmd = vector<double>(size, velocity * 0.12 * (dir == fwd ? 1.0 : -1.0));
		return *this;
	}
	//spinVolt a specific motor	
	chain_method spinVolt(directionType dir, int velocity, int n) {
		setPtoAllowed();
		if (allowedMotors[n]) {
			m[n]->spin(dir, velocity * 12.0 / 100.0, volt);
		}
		lastVoltCmd[n] = velocity * 0.12 * (dir == fwd ? 1.0 : -1.0);
		return *this;
	}	
	//spin a specific motor
	chain_method seperateSpin(vector<int> speeds) {
		setPtoAllowed();
		int i = 0;
		for (auto n : m) {
			if (allowedMotors[i]) {
				n->spin(fwd, speeds[i], pct);
			}
			lastVoltCmd[i] = (double)speeds[i] * 0.12;
			i++;
		}

		return *this;
	}
	//wait for a certain amount of time
	chain_method wait(int msec) {
		task::sleep(msec);
		return *this;
	}
	//spin opposite directions
	chain_method oppSpin(directionType dir, int velocity) {
		setPtoAllowed();
		int i = 0;
		if (dir == directionType::fwd) {
			repeat(size / 2) {
				spin(fwd, velocity, iterator);
				lastVoltCmd[iterator] = velocity * 0.12;
			}
			startRepeat(size / 2, size) {
				spin(vex::reverse, velocity, i);
				lastVoltCmd[i] = velocity * 0.12;
			}
		}
		else {
			repeat(size / 2) {
				spin(vex::reverse, velocity, iterator);
				lastVoltCmd[iterator] = velocity * 0.12;
			}
			startRepeat(size / 2, size) {
				spin(fwd, velocity, i);
				lastVoltCmd[i] = velocity * 0.12;
			}
		}
		CHAIN;
	}
	//stop all motors
	chain_method stop(brakeType brak) {
		setPtoAllowed();
		int i = 0;
		for (auto n : m) {
			if(allowedMotors[i]){
				n->stop(brak);
			}
			
		}
		lastVoltCmd = vector<double>(size, 0.0);
		return *this;
	}
	void set(brakeType b) {
		for (auto n : m) {
			n->setStopping(b);
		}
	}
	template<class T>
	chain_method operator=(vector<T>& v) {
		m.clear();
		for (auto& l : v) {
			m.push_back((motor_type*)l);
		}
		CHAIN;
	}
	template<class T>
	chain_method operator=(vector<T>&& v) {
		m.clear();
		for (auto& l : v) {
			m.push_back((motor_type*)l);
		}
		CHAIN;
	}
	template<class T, size_t s>
	chain_method operator=(array<T, s>& v) {
		m.clear();
		for (auto& l : v) {
			m.push_back((motor_type*)l);
		}
		CHAIN;
	}
	template<class T, size_t s>
	chain_method operator=(array<T, s>&& v) {
		m.clear();
		for (auto& l : v) {
			m.push_back((motor_type*)l);
		}
		CHAIN;
	}
};




