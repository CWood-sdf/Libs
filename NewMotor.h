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
	vector<motor_type*> m;
	template<typename... args>
	void addMotor(args*... a) {
		m = { a... };
		size = m.size();
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
	motor_type& operator[](int n) {
		return *m[n];
	}
	NewMotor<motor_type>& spin(directionType dir, int velocity, percentUnits v = pct) {
		for (auto n : m) {
			n->spin(dir, velocity, v);
		}
		return *this;
	}
	NewMotor<motor_type>& spin(const directionType& dir, int velocity, int n) {
		m[n]->spin(dir, velocity, pct);
		return *this;
	}
	NewMotor<motor_type>& spinVolt(const directionType& dir, int velocity) {
		for (auto n : m) {
			n->spin(dir, velocity * 0.12, volt);
		}
		return *this;
	}
	NewMotor<motor_type>& spinVolt(directionType dir, int velocity, int n) {
		m[n]->spin(dir, velocity * 12.0 / 100.0, volt);
		return *this;
	}
	NewMotor<motor_type>& seperateSpin(vector<int> speeds) {
		int i = 0;
		for (auto n : m) {
			n->spin(fwd, speeds[i], pct);
		}
		return *this;
	}
	NewMotor<motor_type>& wait(int msec) {
		task::sleep(msec);
		return *this;
	}
	NewMotor<motor_type>& oppSpin(directionType dir, int velocity) {
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
	void stop(brakeType brak) {
		for (auto n : m) {
			n->stop(brak);
		}
	}
	void set(brakeType b) {
		for (auto n : m) {
			n->setStopping(b);
		}
	}
	void stop() {
		for (auto n : m) {
			n->stop();
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




