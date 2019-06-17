#pragma once
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
namespace matrix{


class Matrix{
	public:
		Matrix(int size) : 
			_sizei(size),
			_sizej(size),
			_m(_sizei * _sizej, 0){
		}

		Matrix(int sizei, int sizej) : 
			_sizei(sizei),
			_sizej(sizej),
			_m(_sizei * _sizej, 0)
		{

		}
		
		const int& operator()(int i, int j) const {
			return _m[i * _sizej + j];
		}

		int& operator()(int i, int j) {
			return _m[i * _sizej + j];
		}

		const int size() const {
			return _sizei;
		}
		
		const int sizei() const {
			return _sizei;
		}

		const int sizej() const {
			return _sizej;
		}

		const std::vector<int> neighbours(int i, int j) const {
			int n1 = (i - 1) * _sizej + j - 1;
			int n2 = i * _sizei + j - 1;
			int n3 = (i + 1) * _sizej + j - 1;
			return { _m[n1], _m[n1 + 1], _m[n1 + 2],
				 _m[n2], _m[n2 + 2],
				 _m[n3], _m[n3 + 1], _m[n3 + 2]};
		}

		const std::vector<int> data() const {
			return _m;
		}

		void rotateCol(int col, int n){
			bool back = n < 0 ? true : false;
			n = n < 0 ? -n : n;
			for(int j = 0; j < n; ++j){	
				if(back){
					int tmp = (*this)(_sizei - 1, col);
					for(int i = _sizei-2; i >= 0; --i){
						int t = (*this)(i, col);
						(*this)(i, col) = tmp;
						tmp = t;
					}
					(*this)(_sizei-1, col) = tmp;
				} else {
					int tmp = (*this)(0, col);
					for(int i = 1; i < _sizei; ++i){
						int t = (*this)(i, col);
						(*this)(i, col) = tmp;
						tmp = t;
					}
					(*this)(0, col) = tmp;
				}
			}
		}
		
		void rotateRow(int row, int n){
			bool back = n < 0 ? true : false;
			n = n < 0 ? -n : n;
			for(int j = 0; j < n; ++j){
				if(back){
					int tmp = (*this)(row, _sizej -1);
					for(int i = _sizej - 2; i >= 0; --i){
						int t = (*this)(row, i);
						(*this)(row, i) = tmp;
						tmp = t;
					}
					(*this)(row, _sizej -1) = tmp;
				} else {
					int tmp = (*this)(row, 0);
					for(int i = 1; i < _sizej; ++i){
						int t = (*this)(row, i);
						(*this)(row, i) = tmp;
						tmp = t;
					}
					(*this)(row, 0) = tmp;
				}
			}
		}

	private:
		int _sizei;
		int _sizej;
		std::vector<int> _m;
};

	std::ostream& operator<<(std::ostream& out, const Matrix& m){
		for(int i = 0; i < m.sizei(); ++i){
			out << "[";
			for(int j = 0; j < m.sizej(); ++j){
				if(m(i,j) >0 ){
				out <<  m(i,j) << " ";
				} else {
					out << "  ";
				}
			}
			out << "]" << std::endl;
		}
		return out;
	}
}
