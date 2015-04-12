#pragma once

#include "stdafx.h"
#include "lbp.h"
#include "sift_bow.h"

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
		sum += cnt[i];
	}
	for (int i=0; i<256/N; i++){
		cnt[i] = cnt[i] / sum * 256;
	}
	return cnt;
}


