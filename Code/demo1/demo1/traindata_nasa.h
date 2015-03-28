#pragma once

#include"demo1.h"
#include"nca.h"
#include"feature.h"
#include<direct.h>

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
	
	int show_sub(){
		if (!success) return -1;
		return _show( left, right, up, down );
	}
	int show_full(){
		if (!success) return -1;
		return _show( 0, src[0].cols, 0, src[0].rows );
	}
	//
	int _show(int L, int R, int U, int D){
		if (!success) return -1;
		if (L<0 || L>src[0].cols || R<0 || R>src[0].cols || U<0 || U>src[0].rows || D<0 || D>src[0].rows) return -1;

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
		left = (int) (L/0.6 + Size/2);
		right = (int) (R/0.6 + Size/2);
		up = (int)(-U/0.6 + Size/2);
		down =  (int)(-D/0.6 + Size/2);
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
	
	void save(string path, int id){
		for (int i=0; i<9; i++){
			char buf[20];
			itoa(id, buf, 10);
			_mkdir( (path + buf).c_str() );
			string filename = path + buf + "//" + names[i].substr(21,4)+".jpg";
			//cout<<filename<<endl;
			Mat t(src[i], Rect(left, up, right - left, down - up ) );
			imwrite(filename.c_str() , t);
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
			int key = f.show_sub();
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
	string check;
	cout<<"enter \"start\" to continue"<<endl;
	cin>>check;
	if ( check!="start" ) return ;

	string path = "..//..//..//data//TrainSet//";
	vector<string> w;
	ifstream in;
	in.open("anno_nasa.good");
	char buf[10240];
	while( in.getline(buf, 10240) ){
		string t = buf;
		if ( t.length()>0 && t[0]=='#' ) continue;
		w.push_back(t);
	}
	in.close();

	ofstream os;
	os.open(path+"Annotation.txt");
	string outstr = "#id,img1,img2,...,img9,{Flare,Coronal Hole,Sunspot},left,up,right,down,";
	os<<outstr<<endl;

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

		f.save(path+"img//", i);
		
		cout<< (double)i/N *100<<"%" <<endl;

		os<<i<<","<<w[i]<<endl;
		
		//if(i>=2) break;
		
	}
	os.close();

}


class SunFrame : public AnnoFrameBase{	
	static const int Size = 4096;
	static string path;
public:
	vector<double> feature;
	string id;
	void set(string str){
		vector<string> strs;
		int i=-1, j=-1;		
		while( ( j = str.find(",", i+1) ) != string::npos || ( j = str.find("\n", i+1) ) != string::npos ){
			string t = str.substr(i+1, j-i-1);
			//cout<<t<<endl;
			strs.push_back(t);
			i = j;			
		}
		i=0;
		id = strs[i++];
		for ( ; i<10; i++ ){
			names[i-1] = strs[i].substr(21,4)+".jpg";
		}
		concept = strs[i++];
		
		double L = atof( strs[i++].c_str() );
		double U = atof( strs[i++].c_str() );
		double R = atof( strs[i++].c_str() );
		double D = atof( strs[i++].c_str() );
		//assert: left<right, up<down
		left = (int) (L/0.6 + Size/2);
		right = (int) (R/0.6 + Size/2);
		up = (int)(-U/0.6 + Size/2);
		down =  (int)(-D/0.6 + Size/2);
		//cout<<left<<" " <<right<<" "<<up<<" "<<down<<endl;
		success = false;
	}

	void read_img(){
		string filenames[9];
		bool succ[9];
		for (int i=0; i<9; i++){
			filenames[i] = path + id + "//" + names[i];
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
	void calc_feature(){
		feature.clear();
		vector<double> res;
		for (int i=0; i<9; i++){
			res = get_ColorMoment_from_mat( gray[i] );
			for (double t : res) feature.push_back( t );
			res = get_texture_from_mat( gray[i] );
			for (double t : res) feature.push_back( t );
		}

		/*
		for (int i=0; i<9; i++){
			for (int j=0; j<3+16; j++){
				if ( j<7 ) cout<<feature[i*19 + j]<<" ";
			}
			cout<<endl;
		}
		*/

	}
};
string SunFrame::path = "..//..//..//data//TrainSet//img//";

void calc_train_feature(){
	string path = "..//..//..//data//TrainSet//";
	vector<string> w;
	ifstream in;
	in.open( path + "Annotation.txt" );
	char buf[10240];
	while( in.getline(buf, 10240) ){
		string t = buf;
		if ( t.length()>0 && t[0]=='#' ) continue;
		w.push_back(t);
	}
	in.close();

	ofstream os;
	os.open( path + "data.txt" );
	for ( string t : w ){
		SunFrame s;
		s.set(t);
		s.read_img();
		s.calc_feature();
		
		//int key = s.show_full();
		cout<<s.id<<endl;
		//if ( 0x1b == key ) break;

		if ( s.concept == "Flare" ) os<<"1,";		
		else if ( s.concept == "Coronal Hole" ) os<<"2,";
		else if ( s.concept == "Sunspot" ) os<<"3,";
		else continue;
		for ( double f : s.feature ){
			os<<f<<",";
		}
		os<<endl;

		//break;
	}
	os.close();
	
}

void train(){	
	string path = "..//..//..//data//TrainSet//";
	ifstream in;
	in.open( path + "data.txt" );
	
	vector<int> label, train_label, test_label;
	vector< vector<double> > x, train_x, test_x;
	int N=0, D;

	char* buf = new char[1024000];
	while( in.getline(buf, 1024000) ){
		string str = buf;
		if ( str.length()>0 && str[0]=='#' ) continue;
		N++;
		vector<string> strs;
		int i=-1, j=-1;		
		while( ( j = str.find(",", i+1) ) != string::npos || ( j = str.find("\n", i+1) ) != string::npos ){
			string t = str.substr(i+1, j-i-1);
			//cout<<t<<endl;
			strs.push_back(t);
			i = j;
		}
		label.push_back( atoi(strs[0].c_str()) );
		D = strs.size() - 1;
		vector<double> tmp;
		for (int i=1; i<=D; i++){
			tmp.push_back( atof(strs[i].c_str()) );
		}
		x.push_back(tmp);
	}
	in.close();
	delete []buf;

	/*
	for (int i=0; i<3; i++){
		cout<<label[i]<<endl;
		for (int j=0; j<D; j++){
			cout<<x[i][j]<<",";
		}
		cout<<endl;
	}*/
	srand(19921031);
	for (int i=0; i<N; i++){
		//if ( rand()%10 <=  7 )
		if ( i%100 < 50 )
		{
			test_label.push_back(  label[i] );
			test_x.push_back(  x[i] );
		}else{
			train_label.push_back(  label[i] );
			train_x.push_back(  x[i] );
		}
	}
	
	cout<<"train size: "<<train_label.size()<<" "<<train_x.size()<<endl;
	cout<<"test size: "<<test_label.size()<<" "<<test_x.size()<<endl;	
	
	vector< vector<double> > A = vector< vector<double> >(D, vector<double>(D, 0) );
	for (int i=0; i<D; i++) A[i][i] = 1;

	double test_rate = nca_debug::test(train_x, train_label, test_x, test_label, A);
	cout<<"simple knn, accuracy is: "<<test_rate<<endl;
	//nca_debug::wine_demo();

}