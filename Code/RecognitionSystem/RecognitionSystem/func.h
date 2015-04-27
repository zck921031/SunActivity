#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#endif

#include<cmath>
#include<vector>
#include<set>
#include<map>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<cassert>
#include<sstream>
#include<ctime>
#include<thread>
#include<cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;
using namespace std;


const string modal[] = {"0094","0131","0171","0193","0211","0304","0335","1600","1700"};
const string featureType[] = {"colorHist", "siftHist", "lbpHist"};
//const string CONCEPT = "Sunspot";
//const string CONCEPT = "Flare";


vector<string> SplitStringByChar( string str, char c ){
	vector<string> ret;
	int p = 0, q;
	do
	{
		q = str.find(c, p);
		if ( q>=p )
			ret.push_back( str.substr(p, q-p) );
		else
			ret.push_back( str.substr(p) );
		p = q + 1;
	}while( q != -1 );
	return ret;
}

template<class Tp>
vector< vector<Tp> > csvread(string filename){
	vector< vector<Tp> > g;
	ifstream is(filename);
	string tmp;
	while( is>>tmp ){
		vector<string> strs = SplitStringByChar(tmp, ',');
		vector<Tp> elem;
		for ( string t : strs ){
			Tp key = atof( t.c_str() );
			elem.push_back(key);
		}
		g.push_back(elem);
	}
	is.close();
	return g;
}

/**
*  矩形坐标从OpenCV坐标系转到lmsal坐标系
*/
template<class TP>
void CvAxis2LmsalAxis(TP &left, TP &up, TP &right, TP &down, double Size=4096)
{
	left = (left-Size/2)*0.6;
	right = (right-Size/2)*0.6;
	up = (Size/2-up)*0.6;
	down = (Size/2-down)*0.6;
}

/**
*  矩形坐标从lmsal坐标系转到OpenCV坐标系
*/
template<class TP>
void LmsalAxis2CvAxis(TP &left, TP &up, TP &right, TP &down, double Size=4096)
{
	left = (int) (left/0.6 + Size/2);
	right = (int) (right/0.6 + Size/2);
	up = (int)(-up/0.6 + Size/2);
	down =  (int)(-down/0.6 + Size/2);
}

/**
 * 读取图片filename并做中值滤波
 */
Mat load_image(string filename){
	Mat img = imread( filename.c_str() );
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);
	medianBlur( gray, gray, 9);
	return gray;
}