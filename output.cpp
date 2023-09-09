#include <iostream>
#include <cmath>
double customlog(double base, double x) {
	return std::log(x) / std::log(base);
}

int main() {
	float x = 0-2.5;
	std::cout <<   std::abs(x) << std::endl;

	return 0;
}
