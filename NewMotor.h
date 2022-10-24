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
template <class motor_type = vex::motor>
class NewMotor {
	int size = 0;
	typedef NewMotor<motor_type>& chain_method;
	vector<motor_type*> m = vector<motor_type*>();
	vector<tuple<bool, vector<pneumatics*>, vector<int>>> pto = {};
	//might as well overallocate, this is just temporary anyway
	vector<bool> allowedMotors = vector<bool>(10, true);
	template<typename... args>
	void addMotor(args*... a) {
		m = { a... };
		size = m.size();
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
	}
	NewMotor(const NewMotor<>&) = default;
	int addPto(pneumatics& p, vector<int> motors, bool desiredState = true) {
		pto.push_back(make_tuple(desiredState, vector<pneumatics*>{&p}, motors));
		return pto.size() - 1;
	}
	int addPto(vector<pneumatics*> p, vector<int> motors, bool desiredState = true) {
		pto.push_back(make_tuple(desiredState, p, motors));
		return pto.size() - 1;
	}
	chain_method setPtoDrive(int ptoIndex){
		//Loop through desired pto and set the pneumatics to the desired state
		for(auto& p : get<1>(pto[ptoIndex])){
			p->set(get<0>(pto[ptoIndex]));
		}
		CHAIN;
	}
	chain_method setPtoRelease(int ptoIndex){
		//Loop through desired pto and set the pneumatics to the opposite of the desired state
		for(auto& p : get<1>(pto[ptoIndex])){
			p->set(!get<0>(pto[ptoIndex]));
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
	}
	
	void stop() {
		for (auto n : m) {
			n->stop();
		}
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
		return *this;
	}
	//spin a specific motor
	chain_method spin(directionType dir, int velocity, int n) {
		setPtoAllowed();
		if(allowedMotors[n]){
			m[n]->spin(dir, velocity, pct);
		}
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
		return *this;
	}
	//spinVolt a specific motor	
	chain_method spinVolt(directionType dir, int velocity, int n) {
		setPtoAllowed();
		if (allowedMotors[n]) {
			m[n]->spin(dir, velocity * 12.0 / 100.0, volt);
		}
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
			}
			startRepeat(size / 2, size) {
				spin(reverse, velocity, i);
			}
		}
		else {
			repeat(size / 2) {
				spin(reverse, velocity, iterator);
			}
			startRepeat(size / 2, size) {
				spin(fwd, velocity, i);
			}
		}
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




