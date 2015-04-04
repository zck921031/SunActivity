#pragma once

#include "stdafx.h"
#include "FrameBase.h"
#include <direct.h>

class SplitImage{

public:
	void work(){

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

		
		_mkdir("img");
		map<string, int> cnt;
		for (string w : imgnames ){
			FrameBase fb;
			fb.setPath("..//..//..//data//regional annotation//img//");
			fb.unserialize(w);
			if ( fb.annotations.empty() ) continue;
			fb.read_image();
			Mat img;
			for ( Region r : fb.annotations ){
				int id = ++cnt[ r.concept ];
				for (int i=0; i<9; i++){
					img = Mat( fb.src[i], Rect(r.left, r.up, r.right-r.left, r.down-r.up) );
					string filename = "img//"+r.concept;
					_mkdir( filename.c_str() );
					stringstream ss;
					ss<<id;
					filename += "//" + ss.str();
					ss.clear();
					_mkdir( filename.c_str() );
					filename += "//" + fb.names[i].substr(21, 4) + ".jpg";
					imwrite(filename, img);
				}
			}
		}
	}
};