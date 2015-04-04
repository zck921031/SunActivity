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
	
	//显示整幅图片，标注区域用矩形框标出来。按空格切换波段，按其他结束。
	int show_with_board(int L=-1, int U=-1, int R=-1, int D=-1){
		if ( -1 == L ){
			L = left; R = right; U = up; D = down;
		}
		if (!success) return -1;
		if (L<0 || L>src[0].cols || R<0 || R>src[0].cols || U<0 || U>src[0].rows || D<0 || D>src[0].rows) 
			return -1;
		while(1){
			for (const Mat elem: src){
				Mat t = elem.clone();
				for ( int c=-8; c<8; c++ )
				{
					for ( int i=L; i<R; i++ ){
						if ( 0<=U+c && U+c<t.rows ) t.at<Vec3b>( U+c, i ) = Vec3b(255, 0, 0);
						if ( 0<=D+c && D+c<t.rows ) t.at<Vec3b>( D+c, i ) = Vec3b(255, 0, 0);
					}
					for ( int i=U; i<D; i++ ){
						if ( 0<=L+c && L+c<t.cols ) t.at<Vec3b>( i, L+c ) = Vec3b(255, 0, 0);
						if ( 0<=R+c && R+c<t.cols ) t.at<Vec3b>( i, R+c ) = Vec3b(255, 0, 0);
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
	//
	int _show(int L, int R, int U, int D, bool ALL = true, bool DESTROY = true){
		if (!success) return -1;
		if (L<0 || L>src[0].cols || R<0 || R>src[0].cols || U<0 || U>src[0].rows || D<0 || D>src[0].rows) return -1;

		//显示9波段图片
		if(ALL)
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
					if (DESTROY) destroyWindow( windowname );
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

	
	vector<double> calc_feature(int L=-1, int U=-1, int R=-1, int D=-1){
		if ( -1==L ){
			L = 0; R = gray[0].cols;
			U = 0; D = gray[0].rows;
		}
		vector<double> feature;
		vector<double> res;
		for (int i=0; i<9; i++){
			Mat img( gray[i], Rect(L,U, R-L, D-U) );
			res = get_ColorMoment_from_mat( img );
			//res = get_Hist_from_mat( img );
			for (double t : res) feature.push_back( t );
			res = get_texture_from_mat( img );
			for (double t : res) feature.push_back( t );
		}
		return feature;
	}

	string serialize(){
		stringstream os;
		for (int i=0; i<9; i++){
			os<<names[i]<<",";
		}
		os<<concept<<",";
		double L = (left-Size/2)*0.6;
		double R = (right-Size/2)*0.6;
		double U = (Size/2-up)*0.6;
		double D = (Size/2-down)*0.6;
		os<<L<<","<<U<<","<<R<<","<<D<<",";
		return os.str();
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

void generate_traindata(string filename, int ID = 0){
	string check;
	cout<<"enter \"start\" to continue"<<endl;
	cin>>check;
	if ( check!="start" ) return ;

	string path = "..//..//..//data//TrainSet//";
	vector<string> w;
	ifstream in;
	in.open(filename.c_str() );
	char buf[10240];
	while( in.getline(buf, 10240) ){
		string t = buf;
		if ( t.length()>0 && t[0]=='#' ) continue;
		w.push_back(t);
	}
	in.close();

	ofstream os;
	os.open(path + "Annotation.txt");
	string outstr = "#id,img1,img2,...,img9,{Flare,Coronal Hole,Sunspot},left,up,right,down,";
	os<<outstr<<endl;

	int N = w.size();
	AnnoFrame f, f_pre;
	bool succ = false;
	for (int i=0; i<N; i++){
		int id = i+ID;
		if ( i>0 ) f_pre.set(w[i-1]);
		f.set( w[i] );
		if (  f.isSameImgTo(f_pre) ){
			f.success = 1;
		}else{
			f.read_img();
			succ = f.success;
		}
		if (!succ) continue;
		f.save(path+"img//", id);
		
		cout<< (double)(i+1)/N *100<<"%" <<endl;

		os<<id<<","<<w[i]<<endl;
		
		//if(i>=2) break;
		
	}
	os.close();

}

void generate_negative_traindata(int begin){

	ifstream in;
	char buf[10240];
	vector<string> work;
	vector<AnnoFrame> a;
	in.open("..//..//..//data//regional annotation//anno_nasa_lmsal.txt");
	while( in.getline(buf, 10240) ){
		AnnoFrame tmp;
		string t = buf;
		if ( t.length()<5 || t[0]=='#' ) continue;
		work.push_back(t);
		tmp.set(t);
		a.push_back(tmp);
	}
	in.close();

	
	ofstream good, bad;
	good.open("anno_nasa.again.good", ios::app);
	vector<string> w;
	string t_pre = "";
	for (string t : work){
		if ( t_pre.substr(0, 8) != t.substr(0, 8) ){
			if ( t.find("#") == string::npos ){
				w.push_back(t);
			}
		}
		t_pre = t;
	}
	int N = w.size();
	cout<<w.size()<<endl;

	for (int i=0; i<100; i++){
		int p = -1;
		for (int j=0; j<9; j++){
			p = w[i].find(",", p+1) + 1;
		}
		good<<w[i].substr(0, p)<<endl;
	}

	in.close();
	good.close();
	
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

	vector<double> calc_feature(int L=-1, int U=-1, int R=-1, int D=-1){
		if ( -1==L ){
			L = 0; R = gray[0].cols;
			U = 0; D = gray[0].rows;
		}
		feature.clear();
		vector<double> res;
		for (int i=0; i<9; i++){
			Mat img( gray[i], Rect(L,U, R-L, D-U) );
			res = get_ColorMoment_from_mat( img );
			//res = get_Hist_from_mat( img );
			for (double t : res) feature.push_back( t );
			res = get_texture_from_mat( img );
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
		return feature;
	}
};
string SunFrame::path = "..//..//AnnotationSystem//AnnotationSystem//img//";

void calc_train_feature(){
	string path = "..//..//..//data//TrainSet//";
	vector<string> w;
	ifstream in;
	in.open( "Annotation.txt" );
	char buf[10240];
	while( in.getline(buf, 10240) ){
		string t = buf;
		if ( t.length()>0 && t[0]=='#' ) continue;
		w.push_back(t);
	}
	in.close();

	w.clear();
	w.push_back("Flare");
	w.push_back("Coronal Hole");
	w.push_back("Filament");
	w.push_back("None");

	ofstream os;
	os.open( "data.txt" );
	for ( string t : w ){
		int i= 0;
		do{
			stringstream ss;
			ss<<++i;
			string prefix = t + "//" + ss.str();
			ss.clear();
			SunFrame s;
			s.set(prefix + ",20110824_162739_4096_0094.jpg,20110824_162811_4096_0131.jpg,20110824_162725_4096_0171.jpg,20110824_162732_4096_0193.jpg,20110824_162738_4096_0211.jpg,20110824_162821_4096_0304.jpg,20110824_162741_4096_0335.jpg,20110824_162818_4096_1600.jpg,20110824_162808_4096_1700.jpg,Coronal Hole,1576,3360,2088,3616,");
			s.concept = t;
			s.read_img();
			if ( !s.success ) break;
			s.calc_feature();

			cout<<prefix<<endl;

			if ( s.concept == "Flare" ) os<<"1,";		
			else if ( s.concept == "Coronal Hole" ) os<<"2,";
			else if ( s.concept == "Filament" ) os<<"3,";
			else if ( s.concept == "None" ) os<<"0,";
			else continue;
			for ( double f : s.feature ){
				os<<f<<",";
			}
			os<<endl;
		}while( true );
		//break;
	}
	os.close();
	
}


void SystemTest(matrix2d train_x, vector<int> train_label, matrix2d A);

void train(int fl=0, int fr=0x3f3f3f3f){
	cout<<"using fl="<<fl<<" fr="<<fr<<endl;
	string path = "..//..//..//data//TrainSet//";
	ifstream in;
	in.open( path + "data.txt" );
	//in.open( "data.txt" );
	
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
			if ( fl <= i && i < fr )
			tmp.push_back( atof(strs[i].c_str()) );
		}
		
		x.push_back(tmp);
	}
	in.close();
	delete []buf;

	D = x[0].size();

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
		
		//if ( !(i>=0&&i<=200 || i>=650&&i<=850 || i>930) ) continue;
		//if ( label[i] == 3 ) continue;
		//if ( label[i] != 1 ) label[i] = -1;
		//if ( rand()%10 <=  6 )
	/*	if ( i<=218 || ( 684<=i && i<=802 ) || (925<=i && i<=1524)  )
		{
			train_label.push_back(  label[i] );
			train_x.push_back(  x[i] );
		}else if ( (546<=i&&i<646)||(848<=i&&i<897)||(1524<=i&&i<1746) ) {
			test_label.push_back(  label[i] );
			test_x.push_back(  x[i] );
		}*/
		if ( (i+1)%400 < 200  )
		{
			train_label.push_back(  label[i] );
			train_x.push_back(  x[i] );
		}else {
			test_label.push_back(  label[i] );
			test_x.push_back(  x[i] );
		}
	}
	
	cout<<"train size: "<<train_label.size()<<" "<<train_x.size()<<endl;
	cout<<"test size: "<<test_label.size()<<" "<<test_x.size()<<endl;	
	
	vector< vector<double> > A = vector< vector<double> >(D, vector<double>(D, 0) );
	for (int i=0; i<D; i++) A[i][i] = 1;



	double test_rate = nca_debug::test(train_x, train_label, test_x, test_label, A);
	cout<<"simple knn, accuracy is: "<<test_rate<<endl;
	//nca_debug::wine_demo();

	A = nca_solve( train_x, train_label, D, 0, 1 );
	test_rate = nca_debug::test(train_x, train_label, test_x, test_label, A);
	cout<<"after dml(NCA), accuracy is: "<<test_rate<<endl;
	
SystemTest(x, label, A);
return ;
	SystemTest(train_x, train_label, A);
}

//This is a demo code for lab.
void SystemTest(matrix2d train_x, vector<int> train_label, matrix2d A){
	
	AnnoFrame s;
	s.set("20110809_035539_4096_0094.jpg,20110809_035613_4096_0131.jpg,20110809_035537_4096_0171.jpg,20110809_035620_4096_0193.jpg,20110809_035616_4096_0211.jpg,20110809_035633_4096_0304.jpg,20110809_035617_4096_0335.jpg,20110809_035642_4096_1600.jpg,20110809_035632_4096_1700.jpg,Flare,-470.0,476.0,573.0,-174.0,");
	s.read_img();


	map<int,int> cnt_label;
	map<int,double> dist;
	for (int i=0; i<(int)train_x.size(); i++ ){
		for (int j=0; j<(int)train_x.size(); j++){
			if ( train_label[i] != train_label[j] ) continue;
			cnt_label[ train_label[i] ]++;
			dist[ train_label[i] ] += nca_debug::distance2( train_x[i], train_x[j] );
		}
	}
	for ( auto &t : dist ){
		t.second /= cnt_label[ t.first ];
	}
	int Cx = 2048, Cy = 2048;
	for (int sz = 512; sz <= 1024; sz*=2){
		for (int i=sz/2; i+sz<=4096; i+=sz){
			for (int j=sz/2; j+sz<=4096; j+=sz){
				Cx = i + sz/2, Cy = j + sz/2;
				if ( (Cx-2048)*0.6*(Cx-2048)*0.6 + (Cy-2048)*0.6*(Cy-2048)*0.6 > 1000*1000  ) continue;

				vector<double> feature;

				feature = s.calc_feature(i, j, i+sz, j+sz);
				
				//feature = s.calc_feature(s.left, s.up, s.right, s.down );

				//int label = nca_debug::classify_knn(train_x, train_label, feature);
				int label = 0, best = -1;
				map<int,int> cnt_tmp;
				for (int k=0; k<(int)train_x.size(); k++){
					if ( nca_debug::distance2(feature, train_x[k] ) < dist[ train_label[k] ] ){
						int tmp = ++cnt_tmp[ train_label[k]];
						if ( best < tmp ){
							best = tmp;
							label = train_label[k];
						}
					}
				}

				//for (double t : feature) cout<<t<<" ";	cout<<endl;
				//cout<<"Concept: "<<label<<endl;
				//s.show_with_board(s.left, s.up, s.right, s.down);
				//for (int k=0; k<train_x.size(); k++){
				//	if ( train_label[k] == 0 )
				//	cout<<k<<" "<<nca_debug::distance2(train_x[k], feature)<<endl;
				//}

				//return ;

				cout<<feature.size()<<endl;
				//if ( label>0 ){
					cout<<"Concept: "<<label<<"  position:("<<i<<","<<j<<")("<<i+sz<<","<<j+sz<<")"<<endl;					
					int key = -1;
					//key = s.show_with_board(i, j, i+sz, j+sz);
					key = s.show_with_board(i, j, i+sz, j+sz);
					if ( key == 0x1b ) return ;
				//}
			}
		}
	}

}

void _match(string imagename, string tempname, int method){	
	Point matchLoc = Point(0,0);
	double matchVal = 1e25;
	Mat img = imread(imagename);
	Mat temp = imread(tempname);
	namedWindow("Match");
	Mat dst;
	resize(img, dst, Size(640, 640) );
	imshow("Match", dst);
	namedWindow("Template");
	Mat sceen;
	double scale = min( 300.0/temp.cols,  300.0/temp.rows );
	resize(temp, sceen, Size(scale*temp.cols, scale*temp.rows) );
	imshow("Template", sceen);

	Rect roi = Rect(0,0,640,640);
	while( min(temp.rows, temp.cols) > 50 ){
		if ( max(temp.rows, temp.cols) <= 640 ){
			Mat result;
			matchTemplate(dst(roi), temp, result, CV_TM_SQDIFF_NORMED);
			double minVal;
			double maxVal;
			Point minLoc;  
			Point maxLoc;
			minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc );
			cout<<minVal<<" "<<maxVal<<endl;
			if( ( method  != CV_TM_SQDIFF && method != CV_TM_SQDIFF_NORMED ) )
			{
				minVal = -maxVal;
				minLoc = maxLoc;
			}
			if ( minVal < matchVal ){
				matchVal = minVal;	
				matchLoc += minLoc;
				roi = Rect( matchLoc.x, matchLoc.y, temp.cols, temp.rows );
			}
		}
		resize(temp, temp, Size(temp.cols * 0.5, temp.rows * 0.5) );
	}
	rectangle( dst, roi, Scalar::all(255), 2, 8, 0 );
	imshow("Match", dst);
		
	waitKey();
}

void match(){

	for (int id = 1; id <= 42; id ++ ){
		//Mat img = imread("..//..//..//data//regional annotation//img//20110809_032425_4096_0171.jpg");
		//"..//..//..//data//regional annotation//img//20110809_042527_4096_0211.jpg"
		stringstream ID;
		ID<<id;
		string concept = "Coronal Hole";
		concept = "Flare";
		string imagename = "..//..//..//data//regional annotation//img//20110809_032425_4096_0171.jpg";
		imagename = "..//..//..//data//regional annotation//img//20120711_083013_4096_0171.jpg";
		imagename = "..//..//..//data//regional annotation//img//20120123_014150_4096_0211.jpg";
		string tempname = "..//..//AnnotationSystem//AnnotationSystem//img//" + concept + "//" + ID.str() + "//0211.jpg";
		_match(imagename, tempname, CV_TM_SQDIFF_NORMED);

	}
}

