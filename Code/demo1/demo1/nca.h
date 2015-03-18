#pragma once

#include"demo1.h"

#define matrix2d vector< vector<double> >

vector< vector<double> > nca_solve(vector<double> x, vector<int> lable, int n, int d, int iter_bound, double step );
void wine_demo();

