#pragma once

#include"demo1.h"

vector< vector<uchar> > read_jpg_to_vector(const char* filename);
//读取图片并转化为灰度图
Mat read_jpg_to_gray_mat(const char* filename);
//计算颜色矩
vector<double> get_ColorMoment_from_mat(const Mat &img);
//计算纹理特征
vector<double> get_texture_from_mat(const Mat &img, int N = 8);