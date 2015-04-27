#pragma once

#include "lbp.h"
#include "sift_bow.h"
#include "func.h"

vector<double> ColorHist(const Mat &img, int N = 16){
	vector<double>cnt(256/N,0);
	double sum = 0;
	for (int i=0; i<img.cols; i++){
		for (int j=0; j<img.rows; j++){
			int k = img.at<uchar>(j, i);
			cnt[ k/N ] += 1.0;
		}
	}
	for (int i=0; i<256/N; i++){
		sum += cnt[i]*cnt[i];
	}
	sum = max(sqrt(sum), 1.0);
	for (int i=0; i<256/N; i++){
		cnt[i] = cnt[i] / sum;
	}
	return cnt;
}

template<class TP>
void println( vector<TP> x, ofstream& os ){
	int N = x.size();
	for (int i=0; i<N; i++){
		os<<x[i];
		if ( N-1 == i ) os<<endl; else os<<",";
	}
}


void load_feature( string path, vector< vector<double> >&x, vector<int>&y, double ratio = 1.0 ){
	vector< vector<double> > xtmp;
	vector<int> ytmp;
	map<int,int> ycnt;
	ifstream is(path + "//concept.txt");
	{
		int t;
		while( is>>t ){
			ytmp.push_back(t);
			ycnt[t]++;
		}
	}
	for ( auto &it : ycnt ){
		it.second = (int)ceil( it.second * ratio );
	}
	int N = ytmp.size();
	xtmp = vector< vector<double> >(N);
	vector<bool> skip(N, 0);
	for (int i=0; i<N; i++){
		if ( ycnt[ ytmp[i] ]-- <= 0 ) skip[i] = true;
		else skip[i] = false;
	}


	for ( string waveName : modal )
	for ( string ftype : featureType )
	{
		string filename = path + "//" + waveName +  "." + ftype + ".txt";
		vector< vector<double> > g = csvread<double>( filename );
		assert( N == g.size() );
		for (int i=0; i<N; i++){
			for ( auto t : g[i] ){
				xtmp[i].push_back(t);
			}
		}
	}

	x.clear(), y.clear();
	for (int i=0; i<N; i++){
		if ( !skip[i] ){
			x.push_back( xtmp[i] );
			y.push_back( ytmp[i] );
		}
	}

}

