#include<iostream>
//#include<bits/stdc++.h>
#include<cmath>
#include<vector>
#include<cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "feature.h"
#include "nca.h"

using namespace cv;
using namespace std;

int main()
{
	wine_demo();
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

