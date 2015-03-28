#pragma once

#include"demo1.h"

#define matrix2d vector< vector<double> >

vector< vector<double> > nca_solve(vector<double> x, vector<int> lable, int n, int d, int iter_bound, double step );
void wine_demo();

namespace nca_debug{
	double test(const matrix2d&train_x, const vector<int>&train_label,const matrix2d&test_x, const vector<int>&test_label, const matrix2d&A);
}
