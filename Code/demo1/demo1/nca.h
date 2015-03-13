#ifndef _CRT_SECURE_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#endif

#include<iostream>
#include<sstream>
#include<cmath>
#include<vector>
#include<cassert>
#include<set>
#include<map>
#include<algorithm>
using namespace std;

#define matrix2d vector< vector<double> >

vector< vector<double> > nca_solve(vector<double> x, vector<int> lable, int n, int d, int iter_bound, double step );
void wine_demo();

