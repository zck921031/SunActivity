#include<iostream>
#include<vector>
#include<cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

vector< vector<uchar> > read_jpg_to_vector(const char* filename);
//��ȡͼƬ��ת��Ϊ�Ҷ�ͼ
Mat read_jpg_to_gray_mat(const char* filename);
//������ɫ��
vector<double> get_ColorMoment_from_mat(const Mat &img);
//������������
vector<double> get_texture_from_mat(const Mat &img, int N = 8);