#include "traindata.h"
using namespace  _TRAINDATA;

//列出path目录下所有以suffix为后缀的文件
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
		ret.push_back(path+file_name);
    }
    closedir(dp);
	return ret;
}

//从xml文件读取lmsal的标注信息，一个标注的所有<key,value>放到一个map里
void process_xml(const string&filename, vector< map<string,string> >&res ){
	string xml;
	FILE *fin = fopen(filename.c_str(), "r");
	char buf[10240];
	while( fgets(buf, 10240, fin)!=NULL ){
		xml += buf;
	}
	fclose(fin);
	int p = -1, q = -1;
	while( ( p=xml.find("<result>", p+1) )!= xml.npos ){
		int q = xml.find("</result>", p);
		string s = xml.substr(p, q-p+9);		
		map<string,string> ac;
		int a=-1,b,c;
		while( (a=s.find("<param", a+1)) != s.npos ){
			b = s.find(">", a);
			c = s.find("</param>", a);
			int x = a+13;
			int len = b-x-1;
			string key = s.substr(x, len);
			string value = s.substr(b+1, c-b-1 );
			ac[key] = value;
		}
		res.push_back(ac);
	}
}

vector< map<string,string> > get_annotation_from_xml(string path){
	vector< map<string,string> > ret;
	vector<string> filenames = list_file(path, "xml");
	for (auto filename : filenames){
		process_xml(filename, ret);
	}
	cout<<ret.size()<<endl;
	return ret;
}


void SunPic::read(){
	string filename;
	int N = end(freqs) - begin(freqs);
	for (int i=0; i<N; i++){
		string freq=freqs[i], sec=secs[i];
		filename = picture_path+string("//")+freq+"//"+string("aia.lev1.")+freq+"A_"+name+"_"+sec+".jpg";
		//cout<<filename<<endl;
		Mat img=imread(filename);
		if (!img.data){
			success = 0;
			cerr<<"can't find file: "+filename<<endl;
			return ;
		}
		Mat res;
		cvtColor(img, res, CV_BGR2GRAY);
		src.push_back(img);
		gray.push_back(res);
	}
	success = 1;
}

int SunPic::show(double left, double right, double up, double down){
	if (!success) return -1;
	
	//注意lmsal的坐标和opencv的坐标是不一样的，我们的数据和lsmal的数据是上下颠倒的，要转换一下。
	int L=(int)(left/0.6 + src[0].cols/2), R=(int)(right/0.6 + src[0].cols/2),
		U=(int)(up/0.6 + src[0].rows/2), D=(int)(down/0.6 + src[0].rows/2);
	if (L<0 || L>=src[0].cols || R<0 || R>=src[0].cols || U<0 || U>=src[0].rows || D<0 || D>=src[0].rows) return -1;
//	//画坐标系，调试用
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
	while(1){
		for (const Mat &elem: src){
			Mat t(elem, Rect(L, D, R-L, U-D) );
			Mat dst;
			double scale = min( 1000.0/t.cols, 600.0/t.rows );
			resize(t, dst, Size( (int)(scale*t.cols), (int)(scale*t.rows) ) );
			namedWindow("1");
			imshow("1", dst);
			int key = waitKey(0);
			if ( 0x20 != key ) return key;
		}
	}
}

void Traindata::read_annotation(){
	vector< map<string,string> > lmsal_annos = get_annotation_from_xml("..//..//..//data//regional annotation//data//");
	for (auto lmsal_anno:lmsal_annos){
		Anno t;
		t.set(lmsal_anno);
		//{t.print();return ;}
		annos.push_back(t);
	}
}