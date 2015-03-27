#pragma once

#include"demo1.h"
#include"feature.h"

class AnnoFrameBase{
public:
	//读取的名字一定要有序{094A, ..., 1700A}
	string names[9];
	//RGB图片数据
	Mat src[9];
	//Gray图片数据
	Mat gray[9];
	//标注区域的坐标
	int left, up, right, down;
	//标注的类别
	string concept;
	//标记是否读入数据
	bool success;
	//按标注格式初始化
	virtual void set(string str) = 0;
	//读取图片
	virtual void read_img() = 0;
	//
	bool operator == ( const AnnoFrameBase &t ) const
	{
		for (int i=0; i<9; i++){
			if ( names[i] != t.names[i] ) return false;
		}
		if ( concept != t.concept ) return false;
		if ( left != t.left ) return false;
		if ( right != t.right ) return false;
		if ( up != t.up ) return false;
		if ( down != t.down ) return false;
		return true;
	}
	//判断两个标注是否用同一张图片
	bool isSameImgTo( const AnnoFrameBase &t ) const {		
		for (int i=0; i<9; i++){
			if ( names[i] != t.names[i] ) return false;
		}
		return true;
	}
	//
	int show(){
		if (!success) return -1;
		int L=left, R=right, U=up, D=down;
		if (L<0 || L>=src[0].cols || R<0 || R>=src[0].cols || U<0 || U>=src[0].rows || D<0 || D>=src[0].rows) return -1;

		//显示9波段图片
		if(true)
		while(1){
			Mat sh( 3*(D-U), 3*(R-L), CV_8UC3 );
			for (int i=0; i<3; i++){
				for (int j=0; j<3; j++){
					int idx = i*3 + j;
					const Mat&elem = src[idx];
					int X = i*(R-L), Y = j*(D-U);
					for (int x=0; x<R-L; x++){
						for (int y=0; y<D-U; y++){
							sh.at<Vec3b>(y + Y, x + X) = elem.at<Vec3b>( y + U, x + L );
						}
					}
				}
			}		
			Mat dst;
			double scale = min( 1200.0/sh.cols, 600.0/sh.rows );
			resize(sh, dst, Size( (int)(scale*sh.cols), (int)(scale*sh.rows) ) );
			string windowname = concept + " " + names[0].substr(0,8);
			namedWindow( windowname );
			imshow( windowname , dst);
			int key = waitKey(0);
			destroyWindow( windowname );
			return key;
		}

		//按空格切换波段，按其他结束
		while(1){
			for (const Mat &elem: src){
				Mat t(elem, Rect(L, U, R-L, D-U) );
				Mat dst;
				double scale = min( 1000.0/t.cols, 600.0/t.rows );
				resize(t, dst, Size( (int)(scale*t.cols), (int)(scale*t.rows) ) );
				string windowname = concept + " " + names[0].substr(0,8);
				namedWindow( windowname );
				imshow( windowname , dst);
				int key = waitKey(0);
				if ( 0x20 != key ){
					destroyWindow( windowname );
					return key;
				}
			}
		}
	}

};


namespace MultiReader{
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
}

class AnnoFrame : public AnnoFrameBase {
	static const int Size = 4096;
public:
	void set(string str){
		vector<string> strs;
		int i=-1, j=-1;		
		while( ( j = str.find(",", i+1) ) != string::npos || ( j = str.find("\n", i+1) ) != string::npos ){
			string t = str.substr(i+1, j-i-1);
			//cout<<t<<endl;
			strs.push_back(t);
			i = j;			
		}
		for (i=0; i<9; i++){
			names[i] = strs[i];
		}
		concept = strs[i++];
		double L = atof( strs[i++].c_str() );
		double U = atof( strs[i++].c_str() );
		double R = atof( strs[i++].c_str() );
		double D = atof( strs[i++].c_str() );

		//assert: left<right, up<down
		left = L/0.6 + Size/2;
		right = R/0.6 + Size/2;
		up = -U/0.6 + Size/2;
		down = -D/0.6 + Size/2;
		//cout<<left<<" " <<right<<" "<<up<<" "<<down<<endl;
		success = false;
	}
	void read_img(){
		bool multireader = true;
		if ( multireader ){
			string filenames[9];
			bool succ[9];
			for (int i=0; i<9; i++){
				 filenames[i] = "..//..//..//data//regional annotation//img//" + names[i];
				succ[i] = false;
			}
			thread t[9];
			for (int i=0; i<9; i++){
				t[i] = thread( MultiReader::read_foo, filenames[i], &src[i], &gray[i], &succ[i] );
			}
			success = true;
			for (int i=0; i<9; i++){
				t[i].join();
				success &= succ[i];
				if ( !src[i].data ) cerr<<"mutilreader bug"<<endl;
				if ( !gray[i].data ) cerr<<"mutilreader bug"<<endl;
			}
		}
		else
		{
			for (int i=0; i<9; i++){
				string filename = "..//..//..//data//regional annotation//img//" + names[i];
				//cout<<filename<<endl;
				Mat img=imread(filename);
				if (!img.data){
					success = 0;
					cerr<<"can't find file: "+filename<<endl;
					return ;
				}
				Mat res;
				cvtColor(img, res, CV_BGR2GRAY);
				src[i] = img;
				gray[i] = res;
			}
			success = true;
		}
	}
};

void checkallimg(){
	AnnoFrame t;

//	t.set("20110806_084427_4096_0094.jpg,20110806_084500_4096_0131.jpg,20110806_084238_4096_0171.jpg,20110806_084258_4096_0193.jpg,20110806_084314_4096_0211.jpg,20110806_084457_4096_0304.jpg,20110806_084353_4096_0335.jpg,20110806_084506_4096_1600.jpg,20110806_084456_4096_1700.jpg,Flare,-384.0,-307.2,-307.2,-384.0,");
//	t.read_img();
//	t.show();
	set<string> checked;
	ifstream in;
	char buf[10240];
	in.open("anno_nasa.good");
	while( in.getline(buf, 10240) ){
		string t = buf;
		checked.insert(t);
	}
	in.close();

	in.open("anno_nasa.bad");
	while( in.getline(buf, 10240) ){
		string t = buf;
		checked.insert(t);
	}
	in.close();

	vector<string> work;
	in.open("..//..//..//data//regional annotation//anno_nasa_lmsal.txt");
	while( in.getline(buf, 10240) ){
		string t = buf;
		if ( t.length()<5 || t[0]=='#' ) continue;
		//skip the checked file.
		if ( checked.count(t) ) continue;
		work.push_back(t);
	}
	in.close();

	
	ofstream good, bad;
	good.open("anno_nasa.good", ios::app);
	bad.open("anno_nasa.bad", ios::app);
	string Concepts[] = {"Flare","Coronal Hole","Sunspot"};
	bool bk = false;
	for ( string concept : Concepts){
		if (bk) break;
		vector<string> w;
		for (string t : work){
			if ( t.find(concept) != string::npos ){
				w.push_back(t);
			}
		}
		int N = w.size();
		AnnoFrame f, f_pre;
		for (int i=0; i<N; i++){
			if ( i>0 ) f_pre.set(w[i-1]);
			f.set( w[i] );
			if (  f.isSameImgTo(f_pre) ){
				f.success = 1;
			}else{
				f.read_img();
			}
			
			cout<<f.concept<<" "<<f.names[0].substr(0,15)<<" ("<<f.left<<"," <<f.up<<","<<f.right<<","<<f.down<<")"<<endl;
			int key = f.show();
			//enter ESC
			if ( 0x1B == key ) {
				bk = true;
				break;
			}
			if ( 'g' == key || 'G' == key || 'f' == key ){
				cout<<"set the annotation good"<<endl;
				good<<w[i]<<endl;
				good.flush();
			}else if ( 'b' == key || 'B' == key || 'j' == key ){
				cout<<"set the annotation bad"<<endl;
				bad<<w[i]<<endl;
				bad.flush();
			}else{
				cout<<"skip the annotation"<<endl;
			}
		}
	}


	in.close();
	good.close();
	bad.close();

	
}

void generate_traindata(){	
	in.open("anno_nasa.good");
	while( in.getline(buf, 10240) ){
		string t = buf;
		checked.insert(t);
	}
	in.close();

}