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


vector<string> SplitStringByChar( string str, char c );


vector< vector<double> > csvread(string filename);


/**
*  ���������OpenCV����ϵת��lmsal����ϵ
*/
template<class TP>void CvAxis2LmsalAxis(TP &left, TP &up, TP &right, TP &down, double Size=4096);

/**
*  ���������lmsal����ϵת��OpenCV����ϵ
*/
template<class TP>void LmsalAxis2CvAxis(TP &left, TP &up, TP &right, TP &down, double Size=4096);

/**
 * ��ȡͼƬfilename������ֵ�˲�
 */
Mat load_image(string filename);



const string modal[] = {"0094","0131","0171","0193","0211","0304","0335","1600","1700"};
const string featureType[] = {"colorHist", "siftHist", "lbpHist"};
//const string CONCEPT = "Sunspot";
//const string CONCEPT = "Flare";

