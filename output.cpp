#include <iostream>
#include <cmath>
double customlog(double base, double x) {
	return std::log(x) / std::log(base);
}

int main() {
	int a = std::pow(2, 2);
	int b = std::sqrt(4);
	int c = std::cos(90);
	float d = 4.2;
	std::cout <<  d << std::endl;

	return 0;
}
