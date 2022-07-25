//#include <iostream>
//#include <functional>
//
//bool FunctionOldSchool(double a, double b) {
//	std::cout << "I'm a function: ";
//	return a < b;
//}
//
//class Functor {
//public:
//	Functor(double start) : m_start{ start } {}
//	bool operator() (double a, double b) {
//		std::cout << "I'm a function: ";
//		return a < b + m_start;
//	}
//
//private:
//	double m_start;
//};
//
//double OtherDummyFunction(std::function<bool(double, double)> param, double a, double b) {
//	return param(a, b) ? a * 10 : b * 10;
//}
//
//int main() {
//
//	//1 with function
//	std::cout << FunctionOldSchool(5, 6) << std::endl;
//
//	//2 with functor
//	Functor fct(10);
//	std::cout << fct(4, 10) << std::endl;
//
//	//3 with lambda
//	const double start = 10;
//	auto lambda = [start](double a, double b) -> bool {
//		std::cout << "I'm a function: ";
//		return a < b;
//	};
//
//	std::cout << "using lambda: " << lambda(10, 5) << std::endl;
//
//	//4
//	std::cout << "Dummy function  with function as param: " << OtherDummyFunction(FunctionOldSchool, 19, 10);
//	std::cout << "Dummy function  with function as param: " << OtherDummyFunction(FunctionOldSchool, 19, 10);
//	std::cout << "Dummy function  with function as param: " << OtherDummyFunction(FunctionOldSchool, 19, 10);
//
//	return 0;
//}