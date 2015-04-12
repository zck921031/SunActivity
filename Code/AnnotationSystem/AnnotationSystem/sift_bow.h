
/**
 * 提取sift特征，并用bag of words模型建立统计直方图
 * API: ?
 * input: ?
 * output: ?
 */
#include <opencv2/features2d/features2d.hpp>
#include<opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>

namespace __SIFT_BOW{

	int teddytest(){
		
		//initModule_nonfree();
		//从文件中读入图像
		Mat src1 = imread("TestPictures//teddy1.jpg");
		//Mat src1 = imread("C://Users//zck//Documents//GitHub//SunActivity//data//regional annotation//img//20110809_075625_4096_0171.jpg");
		//src1 = Mat(src1, Rect(400,400,4096-800,4096-800) );
		

		Mat src2= imread("TestPictures//teddy2.jpg");
		Mat img1, img2;
		resize(src1, img1, Size(src1.cols/3, src1.rows/3 ) );
		resize(src2, img2, Size(src2.cols/3, src2.rows/3 ) );
 
		//如果读入图像失败
		if(img1.empty())
		{
				return -1;
		}
		if(img2.empty())
		{
				return -1;
		}
		//显示图像
		imshow("image before", img1);
		imshow("image2 before",img2);
		//return waitKey();
 
		//sift特征检测
		SiftFeatureDetector  siftdtc;
		vector<KeyPoint>kp1,kp2;
 
		siftdtc.detect(img1,kp1);
		Mat outimg1;
		drawKeypoints(img1,kp1,outimg1);
		imshow("image1 keypoints",outimg1);
		KeyPoint kp;
 
		vector<KeyPoint>::iterator itvc;
		for(itvc=kp1.begin();itvc!=kp1.end();itvc++)
		{
			//cout<<"angle:"<<itvc->angle<<"\t"<<itvc->class_id<<"\t"<<itvc->octave<<"\t"<<itvc->pt<<"\t"<<itvc->response<<endl;
		}
 
		siftdtc.detect(img2,kp2);
		Mat outimg2;
		drawKeypoints(img2,kp2,outimg2);
		imshow("image2 keypoints",outimg2);
 
 
		SiftDescriptorExtractor extractor;
		Mat descriptor1,descriptor2;
		BruteForceMatcher<L2<float>> matcher;
		vector<DMatch> matches;
		Mat img_matches;
		///descriptor1 is N by 128;
		extractor.compute(img1,kp1,descriptor1);
		extractor.compute(img2,kp2,descriptor2);
		
		
		//imshow("desc",descriptor1);
		cout<<endl<<descriptor1.rows<<" "<<descriptor1.cols<<endl;
		//cout<<endl<<descriptor1<<endl;

		matcher.match(descriptor1,descriptor2,matches);
 
		vector<DMatch> showmatches;
		for (auto t : matches){
			if ( t.distance < 10 && showmatches.size()<10 ) {
				showmatches.push_back(t);
			}
			//cout<<t.distance<<endl;
		}
		drawMatches(img1,kp1,img2,kp2, showmatches ,img_matches);
		imshow("matches",img_matches);
 
		/**
		 * Kmeans
		 */
		int ClusterNum = 10;
		BOWKMeansTrainer bowK(ClusterNum, cvTermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 0.1),3,2);  
		bowK.add(descriptor1);
		Mat centers = bowK.cluster();
		cout<<endl<<"< cluster num: "<<centers.rows<<" >"<<endl;  
      
		Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create( "BruteForce" );  
		//vector<DMatch> matches;
		matches.clear();
		descriptorMatcher->match(descriptor1,centers,matches);//const Mat& queryDescriptors, const Mat& trainDescriptors第一个参数是待分类节点，第二个参数是聚类中心;  
		Mat demoCluster;  
		img1.copyTo(demoCluster);
		//为每一类keyPoint定义一种颜色  
		Scalar color[]={CV_RGB(255,255,255),  
		 CV_RGB(255,0,0),CV_RGB(0,255,0),CV_RGB(0,0,255),  
		 CV_RGB(255,255,0),CV_RGB(255,0,255),CV_RGB(0,255,255),  
		 CV_RGB(123,123,0),CV_RGB(0,123,123),CV_RGB(123,0,123)};  
  
  
		for (vector<DMatch>::iterator iter=matches.begin();iter!=matches.end();iter++)  
		{  
			//cout<<"< descriptorsIdx:"<<iter->queryIdx<<"  centersIdx:"<<iter->trainIdx<<" distincs:"<<iter->distance<<" >"<<endl;  
			Point center= kp1[iter->queryIdx].pt;  
			circle(demoCluster,center,2,color[iter->trainIdx],-1);  
		}
		//putText(demoCluster, "KeyPoints Clustering: one color one type",cvPoint(10,30), FONT_HERSHEY_SIMPLEX, 1 ,Scalar :: all(-1));  
		imshow("KeyPoints Clusrtering",demoCluster);

		//imwrite("sift_kmeans_sample.jpg", demoCluster);
		//此函数等待按键，按键盘任意键就返回
		waitKey();
		return 0;

	}


}
