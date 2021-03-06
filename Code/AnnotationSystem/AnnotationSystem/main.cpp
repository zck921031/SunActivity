

#include "stdafx.h"
#include "AnnotationSystem.h"
#include "SplitImage.h"
#include "Feature.h"
#include "Recognition.h"



void test(){
	Region r1(100,500,200,550);
	if ( r1.intersectArea( Region(130, 400, 140, 525) ) != 250 ) cerr<<"test bug 1.1"<<endl;
	if ( r1.intersectArea( Region(130, 400, 140, 500) ) != 0 ) cerr<<"test bug 1.2"<<endl;
	if ( r1.intersectArea( Region(130, 400, 140, 600) ) != 500 ) cerr<<"test bug 1.3"<<endl;
	if ( r1.intersectArea( Region(130, 400, 240, 600) ) != 3500 ) cerr<<"test bug 1.4"<<endl;
	if ( r1.intersectArea( Region(130, 400, 240, 525) ) != 1750 ) cerr<<"test bug 1.5"<<endl;
	if ( r1.intersectArea( Region(1130, 1400, 1240, 1525) ) != 0 ) cerr<<"test bug 1.6"<<endl;
	
	if( SplitStringByChar("123,45,67,8,,91231ad d,", ',').size() != 7 ) cout<<"test bug 2.1"<<endl;
	
	//cout<<r1.serialize()<<endl;
	//r1.unserialize("sb,10,50,20,55");
	//cout<<r1.serialize()<<endl;

	FrameBase fb1;
	fb1.unserialize("20110809_032451_4096_0094.jpg,20110809_032447_4096_0131.jpg,20110809_032425_4096_0171.jpg,20110809_032432_4096_0193.jpg,20110809_032450_4096_0211.jpg,20110809_032457_4096_0304.jpg,20110809_032453_4096_0335.jpg,20110809_032506_4096_1600.jpg,20110809_032520_4096_1700.jpg,Flare,0,0,0,0,Coronal Hole,0,0,0,0");
	fb1.unserialize("20110809_032451_4096_0094.jpg,20110809_032447_4096_0131.jpg,20110809_032425_4096_0171.jpg,20110809_032432_4096_0193.jpg,20110809_032450_4096_0211.jpg,20110809_032457_4096_0304.jpg,20110809_032453_4096_0335.jpg,20110809_032506_4096_1600.jpg,20110809_032520_4096_1700.jpg,Flare,586,552,1171,-71,Coronal Hole,-715,134,-332,-218");
	if ( fb1.serialize() != "20110809_032451_4096_0094.jpg,20110809_032447_4096_0131.jpg,20110809_032425_4096_0171.jpg,20110809_032432_4096_0193.jpg,20110809_032450_4096_0211.jpg,20110809_032457_4096_0304.jpg,20110809_032453_4096_0335.jpg,20110809_032506_4096_1600.jpg,20110809_032520_4096_1700.jpg,Flare,586,552,1171,-71,Coronal Hole,-715,134,-332,-218" ){
		cout<<"test bug 3.1"<<endl;
		//cout<< fb1.serialize()<<endl;
	}
	fb1.unserialize("20110809_082715_4096_0094.jpg,20110809_082725_4096_0131.jpg,20110809_082702_4096_0171.jpg,20110809_082722_4096_0193.jpg,20110809_082714_4096_0211.jpg,20110809_082721_4096_0304.jpg,20110809_082717_4096_0335.jpg,20110809_082754_4096_1600.jpg,20110809_082720_4096_1700.jpg,Flare,-470,476,573,-174,Coronal Hole,-715,134,-332,-218");
	fb1.setPath("..//..//..//data//regional annotation//img//");
	for (auto &t : fb1.annotations){
		LmsalAxis2CvAxis(t.left, t.up, t.right, t.down);
	}
	//cout<<fb1.serialize()<<endl;
	//fb1.read_image();
	//fb1.show_with_board();

	cout<<"All tests finished~"<<endl;
}

int main(){
	test();
	cout<<"hello world"<<endl;

	__SIFT_BOW::test();
	return 0;
	//AnnotationGui t;
	//t.start(5);
	//return 0;

	//string workers[] = {"Sunspot", "Prominences", "Coronal Hole", "Flare" };
	string workers[] = { "Flare", "Coronal Hole", "Sunspot" };
	set<string> cocept;

	for  ( auto __CONCEPT__ : workers ){
		cocept.insert(__CONCEPT__);
		//cocept.insert("No"+__CONCEPT__);
	}

	//SplitImage si;
	//si.work( cocept );

		//Reco reco;
		//reco.GenFeatureToTxt();
		//reco.ReadFeatureFromTxt();

		//__LBP::test();

		//__SIFT_BOW::teddytest();
	
	CNT["Flare"] = 470;
	CNT["NoFlare"] = 509;
	CNT["Sunspot"] = 72;
	CNT["NoSunspot"] = 122;
	CNT["Prominences"] = 30;
	CNT["NoProminences"] = 40;
	CNT["Coronal Hole"] = 108;
	CNT["NoCoronal Hole"] = 125;

	for  ( auto __CONCEPT__ : workers ){
		CONCEPT = __CONCEPT__;
		vector<int> cnt;
		cnt.push_back( CNT[CONCEPT] );
		cnt.push_back( CNT["No"+CONCEPT] );
		//genFeature(cnt);
	}
	
	//genFeature3();

	system("pause");

	return 0;
}