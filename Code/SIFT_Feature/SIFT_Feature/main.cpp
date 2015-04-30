
#include "stdafx.h"
#include "sift_bow.h"
#include "dirent.h"

map<string, string> pars;
set< string > imageNames;


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
	medianBlur( gray, gray, 9);
	return gray;
}


void genSiftdes(Mat img){
	SiftFeatureDetector  siftdtc;
	vector<KeyPoint>kp; 
	siftdtc.detect(img, kp);
	SiftDescriptorExtractor extractor;
	Mat descriptor;
	extractor.compute(img, kp, descriptor);
}

void genDes(){
	vector<string> existed = list_file(pars["siftpath"], ".sift");
	sort( begin(existed), end(existed) );
	for ( string imagefile : imageNames ){
		string siftfile = imagefile.substr( 0, imagefile.length()-3 ) + "sift";
		cout<<siftfile<<endl;
		Mat img = load_image( imagefile );
		namedWindow("1");
		imshow("1", img);

		system("pause");
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