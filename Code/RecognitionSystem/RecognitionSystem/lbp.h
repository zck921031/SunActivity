#pragma once

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

/**
 * ����ͼƬ��lbp��ֵ��ģʽ
 * API: vector<double> res = Lbp59<uchar>(Mat gray_image);
 * input: Mat gray_image
 * output: ��һ����ֵ��ģʽͳ��ֱ��ͼ��59ά��
 */

template <typename _Tp> vector<double> Lbp256(Mat& src, Mat &dst = Mat( Size(0,0), CV_8UC1), bool debug = false  );
template <typename _Tp> vector<double> Lbp59(Mat& src, Mat &dst = Mat( Size(0,0), CV_8UC1), bool debug = false );
