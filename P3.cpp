
#include <vector>
// c++17
#include <variant>
#include <iostream>

struct OrderRtn {
	int id;
};

class ModuleA {
public:
	void onRtnOrder(const OrderRtn& order) {
		std::cout << "handling rtn order " << order.id
			<< " in moduleA." << std::endl;
	}
};

class ModuleB {
public:
	void onRtnOrder(const OrderRtn& order) {
		std::cout << "handling rtn order " << order.id
			<< " in moduleB." << std::endl;
	}
};

class ModuleC {
public:
	void onRtnOrder(const OrderRtn& order) {
		std::cout << "handling rtn order " << order.id
			<< " in moduleC." << std::endl;
	}
};

using ModuleType = std::variant<ModuleA, ModuleB, ModuleC>;

class Api {
	std::vector<ModuleType> modules;
public:
	void addModule(const ModuleType& m) {
		modules.push_back(m);
	}

	void onRtnOrder(const OrderRtn& order) {
		for (auto& m : modules) {
			std::visit([&order](auto& object) { object.onRtnOrder(order); }, m);
		}
	}
};

int main() {
	Api api;
	ModuleA a;
	ModuleB b;
	ModuleC c;
	api.addModule(a);
	api.addModule(b);
	api.addModule(c);

	OrderRtn order;
	order.id = 10;

	api.onRtnOrder(order);

	return 0;
}

