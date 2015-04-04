#pragma once

#include "stdafx.h"

class Region{
	
	

public:
	int left, up, right, down;
	string concept;
	Region(int left=0, int up=0, int right=0, int down=0):left(left),up(up),right(right),down(down){}
	
	bool contain_point(int x, int y)const{
		if ( left<=x && x<=right && up<=y && y<=down ) return true;
		else return false;
	}

	//区域的面积
	int area(){
		return (right-left)*(down-up);
	}
	//两区域相交的面积
	int intersectArea(const Region& t){
		int _x[4] = {left, right, t.left, t.right};
		int _y[4] = {up, down, t.up, t.down};
		
		const int inf = 0x3f3f3f3f;
		int L = inf, R = -inf, U = inf, D = -inf;
		for (int x : _x)
		for (int y : _y)
		{
			if ( this->contain_point(x,y) && t.contain_point(x,y) ){
				L = min(L, x); R = max(R, x);
				U = min(U, y); D = max(D, y);
			}
		}
		if ( inf == L ) return 0;
		return (R-L)*(D-U);
	}

	//@zck, waring, exception may occurred.
	bool unserialize(string str){
		vector<string> arr = SplitStringByChar(str, ',');
		if ( arr.size() < 5 ) return false;
		concept = arr[0];
		stringstream ss;
		ss<<arr[1];	ss>>left;	ss.clear();
		ss<<arr[2];	ss>>up;	ss.clear();
		ss<<arr[3];	ss>>right;	ss.clear();
		ss<<arr[4];	ss>>down;	ss.clear();
		return true;
	}

	//
	string serialize(){
		stringstream os;
		os<<concept<<",";
		os<<left<<","<<up<<","<<right<<","<<down;
		return os.str();
	}
};

