#include "func.h"

#include "stdafx.h"
#include "sto_triple.h"
#include "sto_pair.h"
#include "ctime"

void MDL();

int main(){
	srand( (unsigned)time(NULL) );
	
	MDL();

	system("Pause");

}


/**
 * contraint (x_i, x_j, x_k) : d(x_i, x_k) - d(x_i, x_j) >= 1
 * hinge loss: max(0, 1 + d2(x_i, x_j) - d2(x_i, x_k) ) s.t. (i,j,k) \in R
 * regular: d2(x_i, x_j) s.t. (i,j) \in S
 */



void MDL(){
	vector< vector<double> > xTr, xTe;
	vector<int> yTr, yTe;
	load_feature( RealPath("C://Users//zck//Documents//GitHub//SunActivity//Code//AnnotationSystem//AnnotationSystem//feature//"),
		xTr, yTr, xTe, yTe, 0.5);
	cerr<<xTr.size()<<endl;
	

	if ( xTr.empty() ) return ;
	cout<<xTr.size()<<" "<<xTr[0].size()<<" "<<xTe.size()<<" "<<xTe[0].size()<<endl;

	int D = xTr[0].size();
	retrieval_test(xTr, yTr, xTe, yTe, vector< vector<double> >() );

	cout<<endl;

	
	sto_pair test;
	test.xTe = xTe;
	test.yTe = yTe;

	vector< vector<double> > L = vector< vector<double> >(3, vector<double>(D,0) );
	for (int i=0; i<(int)L.size(); i++) L[i][i] = 1;

	//L = test.train(xTr, yTr,L );
	int N = xTr.size();
	assert(xTr.size() == yTr.size() );
	vector<int> idx(N);
	for (int i=0; i<N; i++){
		idx[i] = i;
	}
	random_shuffle( begin(idx), end(idx) );

	vector< vector<double> > x;
	vector<int> y;
	vector< double > err;
	for (int i=0; i<N; i++){
		x.push_back( xTr[ idx[i] ] );
		y.push_back( yTr[ idx[i] ] );
		cout<<x.size()<<" "<<x[0].size()<<endl;
		if (i>10){
			L = test.train(x, y, L, 3, 50 );
		}
		
		cout<<"iter = "<<i<<endl;
		cout<<xTe[i].size()<<" "<<xTe.size()<<" "<<yTe.size()<<endl;
		double res = retrieval_test(x, y, xTe, yTe, L );
		cout<<endl;
		err.push_back(res);
	}

	
	retrieval_test(xTr, yTr, xTr, yTr, L );
	retrieval_test(xTr, yTr, xTe, yTe, L );

	FILE *fout = fopen( ("pair_"+Concept).c_str(), "a");
	for (int i=0; i<(int)err.size(); i++){
		fprintf(fout, "%.4f", err[i]);
		if ( i<(int)err.size()-1 ) 
			fprintf(fout, "," );
		else
			fprintf(fout, "\n" );
	}
	fclose(fout);
}