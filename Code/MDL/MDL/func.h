#pragma once

#include "stdafx.h"

using namespace std;


string RealPath(string prefix);
vector<string> SplitStringByChar( string str, char c );
vector< vector<double> > csvread(string filename);
void load_feature( string path, vector< vector<double> >&xTr , vector<int>&yTr,
				  vector< vector<double> >&xTe, vector<int>&yTe, double ratio = 0.5 );


vector<double> project(const vector<double>&x_origin, const vector< vector<double> >&L );
double distance2(const vector<double>&x, const vector<double>&y);

int classify(vector< vector<double> >xTr , vector<int>yTr,
				vector<double>xTe, const vector< vector<double> >&L, bool isprojected = false);

double retrieval_test(vector< vector<double> >xTr , vector<int>yTr,
				  vector< vector<double> >xTe , vector<int>yTe, const vector< vector<double> >&L);

void load_feature_ox5( string path, vector< vector<double> >&xTr , vector<int>&yTr,
				  vector< vector<double> >&xTe, vector<int>&yTe, double ratio = 0.5 );

double norm(const vector< vector<double> > & g);


int classify_knn(vector< vector<double> >xTr , vector<int>yTr, const vector<double>&xTe, int K = 5);

