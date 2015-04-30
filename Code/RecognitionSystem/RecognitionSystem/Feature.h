#pragma once

#include "lbp.h"
#include "sift_bow.h"
#include "func.h"


vector<double> ColorHist(const Mat &img, int N = 16);
template<class TP>void println( vector<TP> x, ofstream& os );
void load_feature( string path, vector< vector<double> >&x, vector<int>&y, double ratio = 1.0 );
void load_feature( string path, vector< vector<double> >&x, vector<int>&y, vector<Sift_BOW>&sift_bows, double ratio = 1.0 );

