
#include "stdafx.h"
#include "sift_bow.h"
#include "dirent.h"

map<string, string> pars;
set< string > imageNames;
const bool debug = false;

vector<string> SplitStringByChar( string str, char c ){
	vector<string> ret;
	int p = 0, q;
	do
	{
		q = str.find(c, p);
		if ( q>=p )
			ret.push_back( str.substr(p, q-p) );
		else
			ret.push_back( str.substr(p) );
		p = q + 1;
	}while( q != -1 );
	return ret;
}


vector<string> list_file(string path, string suffix){
	vector<string> ret;
	DIR *dp;
    struct dirent *dirp;
    if((dp=opendir(path.c_str()))==NULL){
        perror("opendir error");
        free(dp);
        return ret;
    }	
    while((dirp=readdir(dp))!=NULL){
        if((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0)) continue;
        string file_name = dirp->d_name;		
		if ( file_name.find_last_of(suffix) != file_name.size() - 1 ) continue;
		ret.push_back(file_name);
    }
    closedir(dp);
	return ret;
}


void init(){
	pars["imagepath"] = "C://Users//zck//Documents//GitHub//SunActivity//data//regional annotation//img//";
	pars["siftpath"] = "C://Users//zck//Documents//GitHub//SunActivity//data//SiftDes//";
	pars["imageNames"] = "C://Users//zck//Documents//GitHub//SunActivity//Code//RecognitionSystem//RecognitionSystem//data//ImageNameByDay.txt";
	imageNames.clear();
	ifstream in( pars["imageNames"] );
	char *buf = new char[1<<20];
	while( in.getline(buf, 1<<20) ){
		if ( *buf == '#' ) continue;
		if ( strlen(buf) < 3 ) continue;
		auto tmp = SplitStringByChar(buf, ',' );
		for ( auto x : tmp ){
			imageNames.insert( x );
		}
	}
	delete []buf;
	in.close();
}

void DoErase(string filename){
	DeleteFile( filename.c_str() );
}


void EraseUselessImage(){
	vector<string> existedImage = list_file( pars["imagepath"], "jpg" );
	vector<string> eraseImage;
	for (string t : existedImage){
		if ( imageNames.count(t) ){
			//cout<<"保留: "<<t<<endl;
		}else{
			eraseImage.push_back(t);
			cout<<"删除: "<<t<<endl;
			//system("pause");
			DoErase( pars["imagepath"] + "//" + t );
		}
	}
	cout<<"已清理: "<<eraseImage.size()<<" 张图片"<<endl;
}

Mat load_image(string filename){
	Mat img = imread( filename.c_str() );
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);
	medianBlur( gray, gray, 7);
	return gray;
}


pair< vector<KeyPoint>, Mat > genSiftdes(Mat img, float scale = 1.0){
	//Mat img;
	//resize(_img, img, Size(_img.cols*scale, _img.rows*scale) );

	int rows=img.rows, cols=img.cols;
	SiftFeatureDetector  siftdtc;
	vector<KeyPoint>kp; 
	Mat mask;
	mask.create( rows, cols, CV_8UC1);
	for (int x = 0; x < rows; x++){
		for (int y = 0; y < cols; y++){
			if ( x>=256*scale && x<=(4096-2*256)*scale && y>=256*scale && y<=(4096-2*256)*scale )
				mask.at<uchar>(x, y) = 1;
			else
				mask.at<uchar>(x, y) = 0;
		}
	}

	//cout<<img.rows<<" "<<img.cols<<endl;
			
	siftdtc.detect(img, kp, mask);

	SiftDescriptorExtractor extractor;
	Mat descriptor;
	extractor.compute(img, kp, descriptor);


	for ( auto &t : kp){
		t.pt.x /= scale; t.pt.y /= scale;
	}
	return make_pair( kp, descriptor );
}

void write2os(vector<KeyPoint>kp, Mat descriptor, ostream &os = cout ){
	
	int N = kp.size(), M = descriptor.cols;
	for (int i=0; i<N; i++){
		os<<kp[i].pt.x<<","<<kp[i].pt.y<<","<<kp[i].angle<<","<<kp[i].response<<endl;
		for (int j=0; j<M; j++){
			os<<descriptor.at<float>(i,j);
			if ( j < M-1 ) os<<",";else os<<endl;
		}
	}

}

void genDes(){
	vector<string> existed = list_file(pars["siftpath"], ".sift");
	sort( begin(existed), end(existed) );
	for ( string imagefile : imageNames ){
		string siftfile = imagefile.substr( 0, imagefile.length()-3 ) + "sift";

		if ( binary_search( begin(existed), end(existed), siftfile  ) ){
			if (debug) {cout<<"skip "<<siftfile<<endl;}
			continue;
		}

		cout<<"solve: "<<siftfile<<endl;

		Mat img = Mat(load_image( pars["imagepath"] + imagefile ) );
		
		//img = Mat(img, Rect(1600-256, 1600-256, 300+256, 300+256) );
		auto res = genSiftdes( img, 1.0 );
		vector<KeyPoint>kp = res.first;
		Mat descriptor = res.second;

		if ( debug ){
			
			//write2os(kp, descriptor);
			Mat sceen, sceen2;			
			drawKeypoints(img, kp, sceen);
			resize(sceen, sceen2, Size(512,512) );
			namedWindow("1");
			imshow("1", sceen2);
			waitKey();
		}
		
		ofstream os( pars["siftpath"] + "//" + siftfile );
		write2os(kp, descriptor, os);
		os.close();
		
		cout<<" save " << descriptor.rows<<" descriptors "<<endl;
		//system("pause");
	}

}

int main()
{

	init();
	cout<<imageNames.size()<<endl;

	imageNames.clear();
	for ( auto &t : list_file( pars["imagepath"], "jpg" ) ){
		imageNames.insert( t );
	}
	cout<<imageNames.size()<<endl;

	//EraseUselessImage();

	genDes();

	system("pause");
}