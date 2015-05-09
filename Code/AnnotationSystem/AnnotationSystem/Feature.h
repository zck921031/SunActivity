#pragma once

#include "stdafx.h"
#include "lbp.h"
#include "sift_bow.h"
#include <direct.h>

vector<double> ColorHist(const Mat &img, int N = 16){
	vector<double>cnt(256/N,0);
	double sum = 0;
	for (int i=0; i<img.cols; i++){
		for (int j=0; j<img.rows; j++){
			int k = img.at<uchar>(j, i);
			cnt[ k/N ] += 1.0;
		}
	}
	for (int i=0; i<256/N; i++){
		sum += cnt[i]*cnt[i];
	}
	sum = max(sqrt(sum), 1.0);
	for (int i=0; i<256/N; i++){
		cnt[i] = cnt[i] / sum;
	}
	return cnt;
}
template<class TP>
void println( vector<TP> x, ofstream& os ){
	int N = x.size();
	for (int i=0; i<N; i++){
		os<<x[i];
		if ( N-1 == i ) os<<endl; else os<<",";
	}
}


int genFeature( vector<int> cnt ){
	string path = "feature//"+CONCEPT + "//";
	_mkdir( path.c_str() );
	string concepts[] = {CONCEPT, "No"+CONCEPT };
	//int cnt[] = {3, 3};
	int total = 0;for (auto t:cnt) total+=t;
	if ( !total ){
		cout<<"No image need to be processed."<<endl;
		return -1;
	}
	
	for (string wave : WaveNames ){		
		for ( int c=0; c<2; c++ ){			
			string concept = concepts[c];
			for (int ID=1; ID<=cnt[c]; ID++){				
				stringstream ss;
				ss<<ID;
				string imageName = "img//"+concept + "//" + ss.str() + "//"+wave + ".jpg";
				ifstream in;
				in.open(imageName);
				if ( in.fail() ) cerr<<"no file: "<<imageName<<endl;
				in.close();
			}
		}
	}


	for (string wave : WaveNames ){

		Sift_BOW sift_bow;
		sift_bow.clear();

		ofstream colorHistos, lbpHistos, siftHistos, conceptos, siftCenteros;
		colorHistos.open( path + wave+".colorHist.txt" );
		lbpHistos.open( path + wave+".lbpHist.txt" );
		siftHistos.open( path + wave+".siftHist.txt" );
		conceptos.open( path + "concept.txt" );
		siftCenteros.open( path + wave+".center.txt");
		int count = 0;
		for ( int c=0; c<2; c++ ){
			string concept = concepts[c];
			
			for (int ID=1; ID<=cnt[c]; ID++){
				count++;
				stringstream ss;
				ss<<ID;
				string imageName = "img//"+concept + "//" + ss.str() + "//"+wave + ".jpg";
				Mat image = load_image(imageName);
				
				sift_bow.addimage(image);

				println( ColorHist(image), colorHistos );
				println( Lbp59<uchar>(image), lbpHistos );
				println( vector<int>(1,c==0?1:-1), conceptos );

				cout<<"wave: "<<wave<<"  "<<(count*100.0)/(total+0.0)<<"%"<<endl;
				//imshow("test",img);waitKey();//return waitKey();
				
			}
		}

		sift_bow.cluster(50);


		for ( int i=0; i<sift_bow.centers.rows; i++){
			//cout<<sift_bow.centers<<endl;
			vector<double> t;
			for (int j=0; j<sift_bow.centers.cols; j++){
				t.push_back(  sift_bow.centers.at< float >(i,j) );
			}
			println( t, siftCenteros );
		}
		//system("Pause");return 0;

		for (int i=0; i<total; i++){
			println( sift_bow.siftHist(i), siftHistos );
		}

		colorHistos.close();
		lbpHistos.close();
		siftHistos.close();
		conceptos.close();
		siftCenteros.close();
	}
	cout<<"finished~"<<endl;
	return 0;
}

int genFeature2( string concepts[] )
{
	string path = "feature//3//";
	_mkdir( path.c_str() );
	map<string, int> id;
	int total = 0;
	for (int i=0; i<3; i++){
		total+=CNT[ concepts[i] ];
		id[ concepts[i] ] = i+1;
	}
	if ( !total ){
		cout<<"No image need to be processed."<<endl;
		return -1;
	}
	
	for (string wave : WaveNames ){
		for ( int c=0; c<3; c++ ){
			string concept = concepts[c];
			for (int ID=1; ID<=CNT[ concept ]; ID++){
				stringstream ss;
				ss<<ID;
				string imageName = "img//"+concept + "//" + ss.str() + "//"+wave + ".jpg";
				ifstream in;
				in.open(imageName);
				if ( in.fail() ) cerr<<"no file: "<<imageName<<endl;
				in.close();
			}
		}
	}


	for (string wave : WaveNames ){

		Sift_BOW sift_bow;
		sift_bow.clear();

		ofstream colorHistos, lbpHistos, siftHistos, conceptos, siftCenteros;
		colorHistos.open( path + wave+".colorHist.txt" );
		lbpHistos.open( path + wave+".lbpHist.txt" );
		siftHistos.open( path + wave+".siftHist.txt" );
		conceptos.open( path + "concept.txt" );
		siftCenteros.open( path + wave+".center.txt");
		int count = 0;
		for ( int c=0; c<3; c++ ){
			string concept = concepts[c];
			
			for (int ID=1; ID<=CNT[ concept ]; ID++){
				count++;
				stringstream ss;
				ss<<ID;
				string imageName = "img//"+concept + "//" + ss.str() + "//"+wave + ".jpg";
				Mat image = load_image(imageName);
				
				sift_bow.addimage(image);

				println( ColorHist(image), colorHistos );
				println( Lbp59<uchar>(image), lbpHistos );
				println( vector<int>(1, id[concept] ), conceptos );

				cout<<"wave: "<<wave<<"  "<<(count*100.0)/(total+0.0)<<"%"<<endl;
				//imshow("test",img);waitKey();//return waitKey();
				
			}
		}

		sift_bow.cluster(50);


		for ( int i=0; i<sift_bow.centers.rows; i++){
			//cout<<sift_bow.centers<<endl;
			vector<double> t;
			for (int j=0; j<sift_bow.centers.cols; j++){
				t.push_back(  sift_bow.centers.at< float >(i,j) );
			}
			println( t, siftCenteros );
		}
		//system("Pause");return 0;

		for (int i=0; i<total; i++){
			println( sift_bow.siftHist(i), siftHistos );
		}

		colorHistos.close();
		lbpHistos.close();
		siftHistos.close();
		conceptos.close();
		siftCenteros.close();
	}
	cout<<"finished~"<<endl;
	return 0;
}

int genFeature3( ){
	string path = "feature//ox5//";
	_mkdir( path.c_str() );

	string imgpath = "C://Users//zck//Desktop//metric learning//dataset//image//";
	
	string concepts[] = {"airplanes_side", "camel", "faces", "guitars", "motorbikes_side"};
	map< int, map<int,string> > imgs;
	int K = 200;

	for ( int c=0; c<5; c++ ){
		string concept = concepts[c];
		for (int ID=1; ID<=K; ID++){
			stringstream ss;
			ss<<ID;
			string tmp = ss.str();
			while ( tmp.length()<4 ) tmp = "0" + tmp;
			string imageName = imgpath + concept + "//" + tmp + ".jpg";
			ifstream in;
			in.open(imageName);
			if ( in.fail() ){
			string imageName = imgpath + concept + "//image_" + tmp + ".jpg";
				
				in.open(imageName);
				if ( in.fail() ){
					cerr<<"no file: "<<imageName<<endl;
					return -1;
				}else{
					imgs[c][ID] = imageName;
				}
			}else{
				imgs[c][ID] = imageName;
			}
			in.close();
		}
	}

	//return 0;

		Sift_BOW sift_bow;
		sift_bow.clear();

		ofstream colorHistos, lbpHistos, siftHistos, conceptos, siftCenteros;
		colorHistos.open( path + "colorHist.txt" );
		lbpHistos.open( path + "lbpHist.txt" );
		siftHistos.open( path + "siftHist.txt" );
		conceptos.open( path + "concept.txt" );
		siftCenteros.open( path + "center.txt");
		int count = 0;
		for ( int c=0; c<5; c++ ){
			string concept = concepts[c];			
			for (int ID=1; ID<=K; ID++){
				count++;
				string imageName = imgs[c][ID];
				Mat image = load_image(imageName, false);
				
				sift_bow.addimage(image);

				println( ColorHist(image), colorHistos );
				println( Lbp59<uchar>(image), lbpHistos );
				println( vector<int>(1, c+1 ), conceptos );

				cout<<"  "<<(count*100.0)/(K*5+0.0)<<"%"<<endl;
				//imshow("test",img);waitKey();//return waitKey();
				
			}
		}

		sift_bow.cluster(200);


		for ( int i=0; i<sift_bow.centers.rows; i++){
			//cout<<sift_bow.centers<<endl;
			vector<double> t;
			for (int j=0; j<sift_bow.centers.cols; j++){
				t.push_back(  sift_bow.centers.at< float >(i,j) );
			}
			println( t, siftCenteros );
		}
		//system("Pause");return 0;

		for (int i=0; i<K*5; i++){
			println( sift_bow.siftHist(i), siftHistos );
		}

		colorHistos.close();
		lbpHistos.close();
		siftHistos.close();
		conceptos.close();
		siftCenteros.close();

	cout<<"finished~"<<endl;
	return 0;
}