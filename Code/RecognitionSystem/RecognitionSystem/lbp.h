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
 * 计算图片的lbp二值化模式
 * API: vector<double> res = Lbp59<uchar>(Mat gray_image);
 * input: Mat gray_image
 * output: 归一化二值化模式统计直方图，59维。
 */

template <typename _Tp> vector<double> Lbp256(Mat& src, Mat &dst = Mat( Size(0,0), CV_8UC1), bool debug = false  );
template <typename _Tp> vector<double> Lbp59(Mat& src, Mat &dst = Mat( Size(0,0), CV_8UC1), bool debug = false );
