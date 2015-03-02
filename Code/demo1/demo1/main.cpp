#include<iostream>
//#include<bits/stdc++.h>
#include<vector>
#include<cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;


Mat read_jpg_to_gray_mat(const char* filename)
{
	Mat img=imread(filename);

	namedWindow("1");
	imshow("1",img);

	Mat ret;
	cvtColor(img, ret ,CV_BGR2GRAY);

	namedWindow("2");
	imshow("2",ret);

	return ret;
}
vector<double> get_ColorMoment_from_mat(const Mat &img)
{
	vector<double> cm(3,0);
	//一阶矩mu
	double mu = 0;
	for (int i=0; i<img.rows; i++)
		for (int j=0; j<img.cols; j++)
		{
			mu += img.at<uchar>(i,j);
		}
	mu /= img.rows*img.cols;

	//二阶矩sigma和三阶矩s
	double sigma=0, s=0;
	for (int i=0; i<img.rows; i++)
		for (int j=0; j<img.cols; j++)
		{
			double tmp = img.at<uchar>(i,j)-mu;
			sigma += tmp*tmp;
			s += tmp*tmp*tmp;
		}
	sigma = sqrt( sigma/ (img.rows*img.cols) );
	s /= (img.rows*img.cols);
	if ( s>0 )
		s = exp( 1.0/3.0 * log(s) );
	else
		s = -exp( 1.0/3.0 * log(-s) );

	cm[0] = mu;
	cm[1] = sigma;
	cm[2] = s;
	return cm;
}
int main()
{
	// 读入一张图片
	Mat img = read_jpg_to_gray_mat("C://Users//zck//Documents//GitHub//SunActivity//Image Annotation//SampleData//latest_512_0211.jpg");

	vector<double> cm = get_ColorMoment_from_mat(img);
	printf("(%.4f,%.4f,%.4f)\n", cm(0), cm(1), cm(2) );


	// 等待按键后窗口自动关闭
	waitKey(0);
}