#pragma once

#include "Feature.h"

class Reconition{
	vector< vector<double> > x;
	vector<int> y;
public:
	Reconition(){

	}
	
	

	double distance2(const vector<double>&x, const vector<double>&y){
		double sum = 0;
		for (int i=x.size()-1; i>=0; i--){
			sum += (x[i]-y[i])*(x[i]-y[i]);
		}
		return sum;
	}

	int classify(const vector<double>& x){


	}


	vector<Rect> recognition( Mat gray[], string CONCEPT ){
		{
			map<int,int> cnt;
			for (int i=0; i<(int)y.size(); i++){
				cnt[ y[i] ]++;
			}
			for ( auto t : cnt ){
				cout<<"("<<t.first<<" , "<<t.second<<")"<<endl;
			}
		}
		int size = 96;
		vector<Rect> ret;
		for (int i=256; i<4096; i+=size )
		for (int j=256; j<4096; j+=size )
		{
			if ( abs(i+size/2-2048)*abs(i+size/2-2048) + abs(j+size/2-2048)*abs(j+size/2-2048) > 1550*1550  ) continue;
			vector<double> x;
			for (int k=0; k<9; k++){
				Mat img = Mat(gray[k], Rect(i,j,size,size) );
				vector<double> res = ColorHist(img );
				for ( double t : res ) x.push_back(t);				
			}
			int cl = classify(x);

			ret.push_back( Rect(i, j, size, size) );
			cout<<"Find "+CONCEPT+" : "<<Rect(i,j,size, size)<<endl;

		}
		return ret;
	}

};