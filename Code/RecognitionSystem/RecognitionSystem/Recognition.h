#pragma once

#include "Feature.h"

class Recognition{
	vector< vector<double> > xTr;
	vector<int> yTr;
	string Concept;
	vector<Sift_BOW> sift_bows;
	map<string,string> pars;
	vector< vector<double> > L;
	int size, board;
public:
	string RealPath(string prefix){
		return prefix + "//" + Concept + "//";
	}
	vector<double> project(const vector<double>&x_origin, const vector< vector<double> >&L ){
		if ( L.empty() ) return x_origin;

		int D = x_origin.size();
		int d = L.size();
		vector<double> x(d, 0);
		for (int i=0; i<d; i++){
			for (int j=0; j<D; j++){
				x[i] += L[i][j]*x_origin[j];
			}
		}
		return x;
	}

	Recognition(string Concept, const map<string,string>&_pars, int size=320, int board=1500):
		Concept(Concept),pars(_pars),size(size),board(board)
	{
		string realpath = RealPath( pars["featurepath"] );

		load_feature( realpath, xTr, yTr, sift_bows, 0.5 );

		L.clear();
		if ( "Euclidean" != pars["distanceName"] ){
			string distanceName = "mmLMNN_" + Concept + ".txt";

			L = csvread(pars["distancepath"] + "//" + distanceName );
			for (int i=xTr.size()-1; i>=0; i--){
				xTr[i] = project(xTr[i], L);
			}
		}


	}
	
	

	double distance2(const vector<double>&x, const vector<double>&y){
		double sum = 0;
		for (int i=x.size()-1; i>=0; i--){
			sum += (x[i]-y[i])*(x[i]-y[i]);
		}
		return sum;
	}

	int classify(vector<double> xTe){
		
		xTe = project(xTe, L);

		double sump=0, sumn=0, cntp=0, cntn=0;
		for (int i = xTr.size()-1; i>=0; i--){
			double dis = distance2(xTr[i], xTe);
			if ( 1 == yTr[i] ){
				sump += dis;
				cntp += 1;
			}else{
				sumn += dis;
				cntn += 1;
			}
		}
		sump /= cntp;
		sumn /= cntn;
		return sump < 1.0*sumn;
	}

	vector<double> genMMfeature(Mat _img, Sift_BOW sift_bow){
		//!for speed up
		Mat img;
		resize(_img, img, Size(_img.cols/4, _img.rows/4) );

		vector<double> h, res;

		res = ColorHist(img);
		for( auto &t:res ) h.push_back(t);

		res = sift_bow.siftHist(img);
		for( auto &t:res ) h.push_back(t);

		res = Lbp59<uchar>(img);
		for( auto &t:res ) h.push_back(t);

		return h;
	}

	vector<Rect> recognition( vector<Mat> gray ){
		
		vector<Rect> ret;
		int cnt=0;
		#pragma omp parallel for
		for (int i=0; i<4096; i+=size )
		#pragma omp parallel for
		for (int j=0; j<4096; j+=size )
		{
			if ( (i+size/2-2048)*(i+size/2-2048) + (j+size/2-2048)*(j+size/2-2048) > board*board  ) continue;
			vector<double> xTe;
			for (int k=0; k<9; k++){
				Mat img = Mat(gray[k], Rect(i,j,size,size) );
				vector<double> res = genMMfeature(img, sift_bows[k] );
				for ( double t : res ) xTe.push_back(t);
			}
			int cl = classify(xTe);

			#pragma omp critical
			if( cl > 0 ){
				ret.push_back( Rect(i, j, size, size) );
				//cout<<"Find "+Concept+" : "<<Rect(i,j,size, size)<<endl;
			}
			cnt++;
		}
		CString cs;
		cs.Format( _T("%d"), cnt);
		//MessageBox( NULL, cs, _T("Count Windows"), MB_OK );

		return ret;
	}
	
	vector<Rect> recognition( vector<String> names ){
		if ( names.size() != 9 ) return vector<Rect>();
		vector<Mat> images( names.size() );

		#pragma omp parallel for
		for( int i=0; i<(int)names.size(); i++ ){
			images[i] = load_image( pars["imagepath"] + "//" + names[i] );
		}

		//return vector<Rect>();

		return recognition(images);
	}
};