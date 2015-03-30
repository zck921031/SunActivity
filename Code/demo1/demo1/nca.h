#pragma once

#include"demo1.h"

#define matrix2d vector< vector<double> >

matrix2d nca_solve(const matrix2d&x, const vector<int>&label, int d, int iter, double step );
void wine_demo();

namespace nca_debug{
	double test(const matrix2d&train_x, const vector<int>&train_label,const matrix2d&test_x, const vector<int>&test_label, const matrix2d&A);
}
