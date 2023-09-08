#include <iostream>
#include <cmath>
double customlog(double base, double x) {
	return std::log(x) / std::log(base);
}

int main() {
	int a = std::pow(2, 2);
	int b = std::sqrt(4);
	std::cout <<  std::log(2) << std::endl;

	return 0;
}
