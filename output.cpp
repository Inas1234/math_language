#include <iostream>
#include <cmath>
#include <ctime>
double customlog(double base, double x) {
	return std::log(x) / std::log(base);
}

int main() {
	std::srand(std::time(NULL));
	float x =  std::rand()%(10.0-1.0+1)+1.0;
	std::cout <<  x << std::endl;

	return 0;
}
