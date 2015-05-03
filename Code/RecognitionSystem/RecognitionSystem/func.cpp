#include "stdafx.h"
#include "func.h"


vector<string> SplitStringByChar( string str, char c ){
	vector<string> ret;
	int p = 0, q;
	do
	{
		q = str.find(c, p);
		if ( q>=p )
			ret.push_back( str.substr(p, q-p) );
		else
			ret.push_back( str.substr(p) );
		p = q + 1;
	}while( q != -1 );
	return ret;
}


vector< vector<double> > csvread(string filename){
	vector< vector<double> > g;
	ifstream is(filename);
	string tmp;
	while( is>>tmp ){
		vector<string> strs = SplitStringByChar(tmp, ',');
		vector<double> elem;
		for ( string t : strs ){
			double key = atof( t.c_str() );
			elem.push_back(key);
		}
		g.push_back(elem);
	}
	is.close();
	return g;
}

/**
*  矩形坐标从OpenCV坐标系转到lmsal坐标系
*/
template<class TP>
void CvAxis2LmsalAxis(TP &left, TP &up, TP &right, TP &down, double Size)
{
	left = (left-Size/2)*0.6;
	right = (right-Size/2)*0.6;
	up = (Size/2-up)*0.6;
	down = (Size/2-down)*0.6;
}

/**
*  矩形坐标从lmsal坐标系转到OpenCV坐标系
*/
template<class TP>
void LmsalAxis2CvAxis(TP &left, TP &up, TP &right, TP &down, double Size)
{
	left = (int) (left/0.6 + Size/2);
	right = (int) (right/0.6 + Size/2);
	up = (int)(-up/0.6 + Size/2);
	down =  (int)(-down/0.6 + Size/2);
}

/**
 * 读取图片filename并做中值滤波
 */
Mat load_image(string filename){
	Mat img = imread( filename.c_str() );
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);
	medianBlur( gray, gray, 7);
	return gray;
}