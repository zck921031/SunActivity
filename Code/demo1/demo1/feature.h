#pragma once

#include"demo1.h"

vector< vector<uchar> > read_jpg_to_vector(const char* filename);
//��ȡͼƬ��ת��Ϊ�Ҷ�ͼ
Mat read_jpg_to_gray_mat(const char* filename);
//������ɫ��
vector<double> get_ColorMoment_from_mat(const Mat &img);
//������������
vector<double> get_texture_from_mat(const Mat &img, int N = 8);