#pragma once

/**
 * 提取sift特征，并用bag of words模型建立统计直方图
 * API: ?
 * input: ?
 * output: ?
 */
#include "cv.h"
#include "highgui.h"
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
using namespace cv;



class Sift_BOW{
	vector<Mat> descs;
	int N, total;
public:
	Mat centers;
	Sift_BOW(){
		N=total=0;		
	}
	~Sift_BOW(){
		descs.clear();
	}
	/**
	 * 使用读取的聚类中心
	 */
	int setCenters( const vector< vector<double> >& g ){
		N = g.size();
		if ( 0 == N ) return -1;
		int M = g[0].size();

		centers = Mat(N,M, CV_32F);
		for (int i=0; i<N; i++){
			for (int j=0; j<M; j++){
				centers.at<float>(i,j) = (float)g[i][j];
			}
		}
		return 0;
	}

	void addimage(Mat img){	
		SiftFeatureDetector  siftdtc;
		vector<KeyPoint>kp; 
		siftdtc.detect(img, kp);
		SiftDescriptorExtractor extractor;
		Mat descriptor;
		extractor.compute(img, kp, descriptor);
		//put the result of input image to vector<Mat>
		descs.push_back( descriptor.clone() );
		total += descriptor.rows;
	}

	/*
	 * input: ClusterNum, number of cluster centers
	 * output: really number of cluster centers
	 */
	int cluster(int ClusterNum = 50){
		if ( total < ClusterNum ){
			ofstream err;
			err.open("err.log");
			err<<"total descriptors less than Cluster Numbers"<<endl;
			err.close();
		}
		ClusterNum = min(ClusterNum, total);
		BOWKMeansTrainer bowK(ClusterNum);
		for (Mat descriptor : descs ){
			if ( !descriptor.empty() ){
				bowK.add(descriptor);
			}
		}
		centers = bowK.cluster();
		cout<<"Cluster Centers: "<<centers.rows<<endl;
		return N=centers.rows;
	}

	vector<double> _siftHist(Mat descriptor){
		vector<double> hist(N,0);
		Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create( "BruteForce" );  
		vector<DMatch> matches;
		matches.clear();
		descriptorMatcher->match(descriptor, centers, matches);//const Mat& queryDescriptors, const Mat& trainDescriptors第一个参数是待分类节点，第二个参数是聚类中心;  
		for (vector<DMatch>::iterator iter=matches.begin();iter!=matches.end();iter++)  
		{
			hist[iter->trainIdx] += 1;
		}
		double sum = 0;
		for ( double &t : hist ) sum = sum + t*t;
		sum = max( sqrt(sum), 1.0 );
		for ( double &t : hist ) t /= sum;
		return hist;
	}
	
	vector<double> siftHist(int id){
		return _siftHist( descs[id] );
	}

	vector<double> siftHist(Mat img){
		SiftFeatureDetector  siftdtc;
		vector<KeyPoint>kp; 
		siftdtc.detect(img, kp);
		SiftDescriptorExtractor extractor;
		Mat descriptor;
		extractor.compute(img, kp, descriptor);
		return _siftHist(descriptor);
	}

	void clear(){
		descs.clear();
		N=total=0;
	}

};

