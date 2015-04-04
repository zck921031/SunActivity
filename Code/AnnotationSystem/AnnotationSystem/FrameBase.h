#pragma once

#include "stdafx.h"
#include "Region.h"

namespace _AnnotationSystem{
	/**
	 *  多线程读取图片并转为灰度图片
	 *  input: image's filename, ...
	 */
	void read_foo(string&filename, Mat*img, Mat*gray, bool*success){
		*img=imread(filename);
		if (! img->data){
			*success = 0;
			cerr<<"can't find file: "+filename<<endl;
			return ;
		}
		cvtColor(*img, *gray, CV_BGR2GRAY);
		*success = true;
	}
};
using namespace _AnnotationSystem;

//基础帧类，用于记录一副图像的所有标注信息
class FrameBase{
public:
	string path;
	//存取的名字一定要有序{0094A, ..., 1700A}
	string names[9];
	//RGB图片数据
	Mat src[9];
	//Gray图片数据
	Mat gray[9];
	vector<Region> annotations;
	//标记是否读入数据
	bool success;
	//读取图片
	void read_image(){
		string filenames[9];
		bool succ[9];
		for (int i=0; i<9; i++){
				filenames[i] = path + names[i];
			succ[i] = false;
		}
		thread t[9];
		for (int i=0; i<9; i++){
			t[i] = thread( read_foo, filenames[i], &src[i], &gray[i], &succ[i] );
		}
		success = true;
		for (int i=0; i<9; i++){
			t[i].join();
			success &= succ[i];
			if ( !src[i].data ) cerr<<"mutilreader bug"<<endl;
			if ( !gray[i].data ) cerr<<"mutilreader bug"<<endl;
		}
	};
	
	
	//bool operator == ( const FrameBase &t ) const
	//{
	//	for (int i=0; i<9; i++){
	//		if ( names[i] != t.names[i] ) return false;
	//	}
	//	if ( annotations != t.annotations  ) return false;
	//	return true;
	//}
	
	
	//int show_sub(){
	//	if (!success) return -1;
	//	return _show( left, right, up, down );
	//}
	//int show_full(){
	//	if (!success) return -1;
	//	return _show( 0, src[0].cols, 0, src[0].rows );
	//}
	//
	//显示整幅图片，标注区域用矩形框标出来。按空格切换波段，按其他结束。
	int show_with_board(vector<Region> arr = vector<Region>() ){
		if ( arr.empty() ){
			arr = annotations;
		}
		if (!success) return -1;
		while(1){
			for (const Mat elem: src){
				Mat t = elem.clone();
				for ( Region elem : arr ){
					int L = elem.left, R = elem.right, U = elem.up, D = elem.down;
					string concept = elem.concept;
					Vec3b color;
					if ( concept == "Flare" ) color = Vec3b(0, 255, 255);
					else if ( concept == "Coronal Hole" ) color = Vec3b(255, 0, 0);
					else if ( concept == "Null" ) color = Vec3b(255, 255, 255);
					for ( int c=-8; c<8; c++ )
					{
						for ( int i=L; i<R; i++ ){
							if ( 0<=U+c && U+c<t.rows ) t.at<Vec3b>( U+c, i ) = color;
							if ( 0<=D+c && D+c<t.rows ) t.at<Vec3b>( D+c, i ) = color;
						}
						for ( int i=U; i<D; i++ ){
							if ( 0<=L+c && L+c<t.cols ) t.at<Vec3b>( i, L+c ) = color;
							if ( 0<=R+c && R+c<t.cols ) t.at<Vec3b>( i, R+c ) = color;
						}
					}
				}
				Mat dst;
				double scale = min( 1000.0/t.cols, 600.0/t.rows );
				resize(t, dst, Size( (int)(scale*t.cols), (int)(scale*t.rows) ) );
				string windowname = "1";
				namedWindow( windowname );
				imshow( windowname , dst);
				int key = waitKey(0);
				if ( 0x20 != key ){
					return key;
				}
			}
		}
	}
	////
	//int _show(int L, int R, int U, int D, bool ALL = true, bool DESTROY = true){
	//	if (!success) return -1;
	//	if (L<0 || L>src[0].cols || R<0 || R>src[0].cols || U<0 || U>src[0].rows || D<0 || D>src[0].rows) return -1;

	//	//显示9波段图片
	//	if(ALL)
	//	while(1){
	//		Mat sh( 3*(D-U), 3*(R-L), CV_8UC3 );
	//		for (int i=0; i<3; i++){
	//			for (int j=0; j<3; j++){
	//				int idx = i*3 + j;
	//				const Mat&elem = src[idx];
	//				int X = i*(R-L), Y = j*(D-U);
	//				for (int x=0; x<R-L; x++){
	//					for (int y=0; y<D-U; y++){
	//						sh.at<Vec3b>(y + Y, x + X) = elem.at<Vec3b>( y + U, x + L );
	//					}
	//				}
	//			}
	//		}		
	//		Mat dst;
	//		double scale = min( 1200.0/sh.cols, 600.0/sh.rows );
	//		resize(sh, dst, Size( (int)(scale*sh.cols), (int)(scale*sh.rows) ) );
	//		string windowname = concept + " " + names[0].substr(0,8);
	//		namedWindow( windowname );
	//		imshow( windowname , dst);
	//		int key = waitKey(0);
	//		destroyWindow( windowname );
	//		return key;
	//	}

	//	//按空格切换波段，按其他结束
	//	while(1){
	//		for (const Mat &elem: src){
	//			Mat t(elem, Rect(L, U, R-L, D-U) );
	//			Mat dst;
	//			double scale = min( 1000.0/t.cols, 600.0/t.rows );
	//			resize(t, dst, Size( (int)(scale*t.cols), (int)(scale*t.rows) ) );
	//			string windowname = concept + " " + names[0].substr(0,8);
	//			namedWindow( windowname );
	//			imshow( windowname , dst);
	//			int key = waitKey(0);
	//			if ( 0x20 != key ){
	//				if (DESTROY) destroyWindow( windowname );
	//				return key;
	//			}
	//		}
	//	}
	//}
	

	void setPath(string str){
		path = str;
	}

	bool unserialize(string str){
		annotations.clear();

		vector<string> arr = SplitStringByChar(str, ',' );		
		int n=arr.size();
		if ( n<9 ) return false;
		int i=0;
		for (i=0; i<9; i++){
			names[i] = arr[i];
		}
		for ( ; i+4<n; i+=5){
			string tmp;
			for (int j=i; j<i+5; j++) {
				tmp += arr[j];
				if ( j<i+4 ) tmp += ",";
			}
			Region region;
			region.unserialize(tmp);
			annotations.push_back(region);
		}

		return true;
	}

	string serialize(){
		stringstream os;
		for (int i=0; i<9; i++){
			os<<names[i]<<",";
		}
		int n = annotations.size();
		for ( int i=0; i<n; i++ ){
			os<<annotations[i].serialize()+",";
		}
		
		string ret = os.str();
		int len = ret.length();
		return ret.substr(0, len-1);
	}

};

