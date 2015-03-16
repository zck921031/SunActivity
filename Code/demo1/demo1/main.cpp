#include<iostream>
//#include<bits/stdc++.h>
#include<cmath>
#include<vector>
#include<cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "feature.h"
#include "nca.h"
#include "traindata.h"

using namespace cv;
using namespace std;


void show_active_regional(){	
	Mat src = imread("C://Users//zck//Desktop//新建文件夹//毕业设计//数据//211//aia.lev1.211A_2011-08-09T06_00_00.jpg");
//	//打坐标，调试用
//	for (int k=-4; k<=4; k++){
//		for (int l=-5; l<=5; l++){
//			for (int i=0; i<src.rows; i++){
//				Vec3b color = Vec3b(0,0,0);
//				if (k==0) color = Vec3b(255,255,255);
//				src.at<Vec3b>(i, 2048+416*k+l) = color;
//				src.at<Vec3b>(2048+416*k+l, i) = color;
//			}
//		}
//		//cout<<2048+300*k<<endl;
//	}
	double left=-470, right=573, up=476, down=-174;
	//注意lmsal的坐标和opencv的坐标是不一样的，我们的数据和lsmal的数据是上下颠倒的，要转换一下。
	int L=left/0.6 + src.cols/2, R=right/0.6 + src.cols/2, U=up/0.6 + src.rows/2, D=down/0.6 + src.rows/2;
	Mat sub(src, Rect(L, D, R-L, U-D) );
	Mat img;
	double scale = min( 1000.0/sub.cols, 600.0/sub.rows );
	resize(sub, img, Size( scale*sub.cols, scale*sub.rows ) );
	namedWindow("1");
	imshow("1", img);
	waitKey(0);
}
int main()
{
	//wine_demo();
	//vector< map<string,string> > anno = get_annotation_from_xml("..//..//..//data//regional annotation//data//");
	show_active_regional();
	return 0;

	// 读入一张图片
	Mat img = read_jpg_to_gray_mat("C://Users//zck//Documents//GitHub//SunActivity//Image Annotation//SampleData//latest_512_0094.jpg");	

	vector<double> cm = get_ColorMoment_from_mat(img);
	cout<<"("<<cm[0]<<","<<cm[1]<<","<<cm[2]<<")"<<endl;

	vector<double> texture = get_texture_from_mat(img);
	for (int i=0; i<(int)texture.size(); i++){
		cout<<texture[i];
		if ( i == (int)texture.size()-1 ) cout<<endl;
		else cout<<" ";
	}
	
	// 等待按键后窗口自动关闭
	waitKey(0);
}

