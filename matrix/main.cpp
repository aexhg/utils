#include "matrix.hpp"
#include <iostream>

int main(){
	using namespace matrix;
	Matrix m(5, 3);

	for(int i = 0; i < m.sizei(); ++i){
		for( int j = 0; j < m.sizej(); ++j){
			m(i,j) = j + 3 * i;
		}
	}

	//std::swap(m(0,1), m(1,0));
	std::cout << m << std::endl;

	m.rotateRow(1, 4);
	std::cout << m << std::endl;

	m.rotateRow(2, 2);
	std::cout << m << std::endl;

	m.rotateCol(1, 2);
	std::cout << m << std::endl;

	m.rotateCol(0, -2);
	std::cout << m << std::endl;
	
	return 0;
}
