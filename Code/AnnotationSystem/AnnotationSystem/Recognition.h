#pragma once

#include "stdafx.h"
#include "Feature.h"

class Reco{
	map<string, int> mp;
	vector< vector<double> > xT;
	vector<int> yT;
public:
	Reco(){
		mp["Flare"] = 1;
		mp["NoFlare"] = -1;

		mp["Coronal Hole"] = 2;
		mp["NoCoronal Hole"] = -2;

		mp["Filament"] = 3;
		mp["NoFilament"] = -3;

		mp["None"] = 0;

		mp["Sunspot"] = 4;
		mp["NoSunspot"] = -4;

		mp["Prominences"] = 5;
		mp["NoProminences"] = -5;
	}
	void ReadFeatureFromTxt(){
		ifstream in;
		in.open("data.txt");
		char *buf = new char[1<<20];
		while( in.getline(buf, 1<<20) ){
			if ( *buf == '#' ) continue;
			if ( strlen(buf) < 3 ) continue;
			vector<string> strs = SplitStringByChar(buf, ',');
			int D = strs.size()-1;
			yT.push_back( atoi( strs[0].c_str() ) );
			vector<double> x;
			for (int i=1; i<=D; i++){
				x.push_back( atof(strs[i].c_str()) );
			}
			xT.push_back(x);
		}
		delete []buf;
		in.close();
		//cout<<"Read feature from txt: data size: "<<xT.size()<<" label size: "<<yT.size()<<" dim: "<<xT[0].size()<<endl;
	}

	void GenFeatureToTxt(){
		vector<string> imgnames;
		ifstream in;
		in.open("annotation.log");
		char *buf = new char[1<<20];
		while( in.getline(buf, 1<<20) ){
			if ( *buf == '#' ) continue;
			if ( strlen(buf) < 3 ) continue;
			imgnames.push_back(buf);
		}
		delete []buf;
		in.close();

		for (string w : imgnames ){
			FrameBase fb;
			fb.setPath("..//..//..//data//regional annotation//img//");
			fb.unserialize(w);
			if ( fb.annotations.empty() ) continue;
			fb.read_image();
			Mat img;
			for ( Region r : fb.annotations ){
				vector<double> x;
				for (int i=0; i<9; i++){
					img = Mat( fb.gray[i], Rect(r.left, r.up, r.right-r.left, r.down-r.up) );
					vector<double> res;
					res = ColorHist(img);
					for ( double t : res ) x.push_back(t);
					//Mat sceen;
					//resize(fb.gray[i], sceen, Size(512,512) );
					//imshow("tset", sceen);
					//waitKey();
				}
				add( r.concept, x, false );
			}
		}
		save();
	}
	
	
	void add(string concept, vector<double>x, bool issave = true ){
		yT.push_back( mp[concept] );
		xT.push_back( x );
		
		if (issave) save();
	}

	void add(Mat gray[], string concept, Rect r){		
		vector<double> x;
		for (int k=0; k<9; k++){
			Mat img = Mat(gray[k], r );
			vector<double> res = ColorHist(img );
			for ( double t : res ) x.push_back(t);			
		}
		add(concept, x, true);
	}

	void save(){
		int N = xT.size();
		ofstream os;
		os.open("data.txt");
		for (int i=0; i<N; i++){
			os<<yT[i];
			for ( double t : xT[i] ){
				os<<","<<t;
			}
			os<<endl;
		}
		os.close();
	}

	double distance2(const vector<double>&x, const vector<double>&y){
		double sum = 0;
		for (int i=x.size()-1; i>=0; i--){
			sum += (x[i]-y[i])*(x[i]-y[i]);
		}
		return sum;
	}

	int classify(const vector<double>& x, string CONCEPT){
		int N = xT.size();
		vector< pair<double,int> > dist(N);
		for (int i=0; i<N; i++){
			dist[i].first = distance2(xT[i], x);
			dist[i].second = yT[i];
			if ( abs(yT[i]) != abs( mp[CONCEPT] ) ) dist[i].first = 1e25;
			//else cout<<dist[i].first<<" "<<yT[i]<<endl;
		}
		sort( begin(dist), end(dist) );
		int K = 5;
		map<int,int> mp;
		int ret = -1, best = -1;
		for (int i=0; i<K; i++){
			int cnt = ++mp[ dist[i].second ];
			if ( cnt>best ){
				best = cnt;
				ret = dist[i].second;
			}
		}
		return ret;
	}


	vector<Rect> recognition( Mat gray[], string CONCEPT ){
		if ( abs( mp[CONCEPT] ) >= 3 ) return vector<Rect>();
		{
			map<int,int> cnt;
			for (int i=0; i<(int)yT.size(); i++){
				cnt[ yT[i] ]++;
			}
			for ( auto t : cnt ){
				cout<<"("<<t.first<<" , "<<t.second<<")"<<endl;
			}
		}
		int size = 512;
		vector<Rect> ret;
		for (int i=256; i<4096; i+=size )
		for (int j=256; j<4096; j+=size )
		{
			if ( abs(i+size/2-2048)*abs(i+size/2-2048) + abs(j+size/2-2048)*abs(j+size/2-2048) > 1400*1400  ) continue;
			vector<double> x;
			for (int k=0; k<9; k++){
				Mat img = Mat(gray[k], Rect(i,j,size,size) );
				vector<double> res = ColorHist(img );
				for ( double t : res ) x.push_back(t);				
			}
			int cl = classify(x, CONCEPT);
			if ( mp[CONCEPT]==cl ){
				ret.push_back( Rect(i/8, j/8, size/8, size/8) );
				cout<<"Find "+CONCEPT+" : "<<Rect(i,j,size/8,size/8)<<endl;
			}
		}
		return ret;
	}

};