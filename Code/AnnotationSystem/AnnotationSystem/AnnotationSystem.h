#pragma once

#include "FrameBase.h"
#include "Region.h"
#include "Frame.h"
#include "stdafx.h"
#include "Recognition.h"
#include "lmsal.h"

void onMouse1(int event,int x,int y,int flags, void* _r );
void onMouse2(int event,int x,int y,int flags, void* _r );



class AnnotationGui{
public:
	Region button[10];
	Mat sceen;
	int day, wave, N;
	vector<string> imgnames;
	string cmd;
	Mat img[9];
	FrameBase fb;
	Region region;	
	Reco reco;
	vector<Rect> recognition_result;
	Lmsal lmsal;
	//reco.ReadFeatureFromTxt();
	void putButton(Region r){
		rectangle(sceen, Point(r.left, r.up), Point(r.right, r.down), Scalar(255,0,0));		
		putText( sceen, r.concept.substr(0,8), Point(r.left, r.down-5),CV_FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 0, 0) );
	}

	void initButton(){
		int H = 50, W = 100;
		button[0] = Region(10, 10, 10+W, 10+H);
		button[0].concept = "Flare";

		button[1] = Region(130, 10, 130+W, 10+H);
		button[1].concept = CONCEPT;

		button[2] = Region(250, 10, 250+W, 10+H);
		button[2].concept = "EXP";

		button[3] = Region(10, 80, 10+W, 80+H);
		button[3].concept = "None";

		button[4] = Region(130, 80, 130+W, 80+H);
		button[4].concept = "No"+CONCEPT;

		button[5] = Region(250, 80, 250+W, 80+H);
		button[5].concept = "Confirm";
		
		button[6] = Region(10, 150, 10+W, 150+H);
		button[6].concept = "Pre";
		
		button[7] = Region(130, 150, 130+W, 150+H);
		button[7].concept = "Next";
		
		button[8] = Region(250, 150, 250+W, 150+H);
		button[8].concept = "Save";

		
		button[9] = Region(10, 220, 10+W, 220+H);
		button[9].concept = "ChangeWave";

		for (int i=0; i<=9; i++){
			putButton( button[i] );
		}

	}

	void show_image(){
		Mat sceen = img[wave].clone();
		putText( sceen, region.concept, Point(50, 50),CV_FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 255, 0) );
		//cout<<wave<<endl;
		rectangle(sceen, Point(region.left/8, region.up/8), Point(region.right/8, region.down/8), Scalar(0,0,255));
		for ( Region r : fb.annotations  ){
			Scalar color =Scalar(0,0,255);
			if ( r.concept == "No"+CONCEPT ){
				color = Scalar(255,0,0);
			}
			if ( r.concept.find(CONCEPT) != string::npos ){
				rectangle(sceen, Point(r.left/8, r.up/8), Point(r.right/8, r.down/8), color );	
			}
		}
		
		for ( Rect t : recognition_result ){
			rectangle(sceen, t,  Scalar(255,255,0) );
		}
		for ( Rect t : lmsal.res ){
			rectangle(sceen, Rect(t.x/8, t.y/8, t.width/8, t.height/8 ),  Scalar(0,255,255) );
			//cout<<t<<endl;
		}


		imshow("Sun", sceen );
		
	}

	void read_image(){
		fb.setPath("..//..//..//data//regional annotation//img//");
		fb.unserialize( imgnames[day] );
		fb.read_image();
		for (int i=0; i<9; i++){
			double scale = 0.125;
			resize(fb.src[i], img[i], Size( (int)(scale*fb.src[i].cols), (int)(scale*fb.src[i].rows) ) );
		}
		cmd = "";
		
		//简单识别
		region.left = 0;
		region.right = 0;
		region.up = 0;
		region.down = 0;
		recognition_result = reco.recognition( fb.gray, CONCEPT );
		//读标注
		lmsal.find( imgnames[day], CONCEPT );

		cout<<"Day: "<<day<<endl;
	}

	void start(int DAY = 0){
		sceen = Mat(280, 380, CV_8UC3);
		for (int i=0; i<380; i++){
			for (int j=0; j<280; j++){
				sceen.at<Vec3b>(j,i) = Vec3b(255, 255, 255);
			}
		}
		//加载已学习的特征
		reco.ReadFeatureFromTxt();
		//加载lmsal标注
		lmsal.read_annotation_from_txt("..//..//demo1//demo1//lmsal.txt");
		
		//gui初始化
		initButton();

	
		namedWindow("Gui");
		imshow("Gui", sceen);


		ifstream in;
		in.open("annotation.log");
		char *buf = new char[1<<20];
		while( in.getline(buf, 1<<20) ){
			if ( *buf == '#' ) continue;
			if ( strlen(buf) < 3 ) continue;
			imgnames.push_back(buf);
		}
		delete []buf;

		day = DAY;
		wave = 0;
		N = imgnames.size();
		cout<<"image size : "<<N<<endl;

		read_image();
		show_image();

		
		cvSetMouseCallback("Gui", onMouse1, this);
		cvSetMouseCallback("Sun", onMouse2, this);

		waitKey();
	}

	void save(){
		ofstream os;
		os.open("annotation.log");
		for ( string w : imgnames ){
			os<<w<<endl;
		}

		os.close();
	}

};

void onMouse1(int event,int x,int y,int flags, void* _r ){
	AnnotationGui *p = (AnnotationGui *)_r;
	if ( event == CV_EVENT_LBUTTONDOWN ){
		//cout<<"("<<x<<","<<y<<")"<<endl;
		for (int i=0; i<10; i++){
			string c = p->button[i].concept;
			if ( p->button[i].contain_point(x,y) )
			{
				cout<<c<<endl;
				if ( i<=4 ) p->region.concept = c;
				if ( c=="Pre" ){
					p->save();
					p->day = max(p->day-1, 0);
					p->read_image();
				}
				if ( c=="Next" ){
					p->save();
					p->day = min(p->day+1, p->N-1);
					p->read_image();
				}
				if ( c=="Save" ) p->save();
				if ( c=="Confirm" ){
					if ( p->region.concept == "" ){
						cout<<"miss concept, forget?"<<endl;
					}else{
						

						if ( p->region.left > p->region.right ) swap(p->region.left, p->region.right);
						if ( p->region.up > p->region.down ) swap(p->region.up, p->region.down);
						
						p->reco.add( p->fb.gray, p->region.concept, 
							Rect( p->region.left, p->region.up, 
								p->region.right - p->region.left - 1, p->region.down - p->region.up - 1 ) );
						//continue;

						p->fb.annotations.push_back( p->region );
						p->imgnames[p->day] = p->fb.serialize();
						cout<<p->region.serialize()<<endl;
						p->save();
						//p->region = Region();
					}
				}
				if ( c=="ChangeWave" ){
					p->wave = (p->wave+1)%9;
				}
				p->show_image();
			}
		}
	}
}

void onMouse2(int event,int x,int y,int flags, void* _r ){
	AnnotationGui *p = (AnnotationGui *)_r;
	if ( event == CV_EVENT_LBUTTONDOWN ){
		p->region.left = x*8, p->region.up = y*8;
	}else if ( event == CV_EVENT_LBUTTONUP ){
		p->region.right = x*8, p->region.down = y*8;
	}
	
	if ( flags == CV_EVENT_FLAG_LBUTTON ){
		p->region.right = x*8, p->region.down = y*8;
		p->show_image();
	}

}

