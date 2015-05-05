
#include "stdafx.h"
#include "func.h"

class sto_pair{

public:
	vector< vector<double> >  xTr, xTe;
	vector<int>  yTr, yTe;
	vector< vector<double> > getGrad1( vector< vector<double> > xTr, vector<int> yTr, const vector< vector<double> > &L )
	{
		int d = L.size(), D=L[0].size(), N=xTr.size();
		vector< vector<double> > g(d, vector<double>(D,0) );
		double subsample = 0.3;
		for (int ii=0; ii<N*subsample; ii++){

			int i = abs( rand() ) % N;

			for (int jj=0; jj<N*subsample; jj++){

				int j = abs( rand() ) % N;

				if ( yTr[i] == yTr[j] ){
					vector<double> x(D, 0);
					for (int k=0; k<D; k++){
						x[k] = xTr[i][k] - xTr[j][k];
					}
					vector<double> tmp(d, 0);

					for (int u=0; u<d; u++){
						for (int v=0; v<D; v++){
							tmp[u] += L[u][v]*x[v];
						}
					}

					for (int u=0; u<d; u++){
						for (int v=0; v<D; v++){
							g[u][v] += 2.0 * tmp[u] * x[v];
						}
					}

				}else{

					if ( distance2( project(xTr[i], L), project(xTr[j],L) )>=1 ) continue;
					vector<double> x(D, 0);
					for (int k=0; k<D; k++){
						x[k] = xTr[i][k] - xTr[j][k];
					}
					vector<double> tmp(d, 0);

					for (int u=0; u<d; u++){
						for (int v=0; v<D; v++){
							tmp[u] += L[u][v]*x[v];
						}
					}

					for (int u=0; u<d; u++){
						for (int v=0; v<D; v++){
							g[u][v] -= 20.0 * tmp[u] * x[v];
						}
					}

				}
			}
		}
		return g;
	}

	double getH1(vector< vector<double> > xTr, vector<int> yTr, const vector< vector<double> > &L){	
		int N=xTr.size(), D=xTr[0].size(), d=L.size();
		for ( auto &t : xTr ){
			t = project(t, L);
		}
		double sum = 0;
		for (int i=0; i<N; i++){
			for (int j=0; j<N; j++){
				if ( yTr[i] == yTr[j] )
					sum += distance2( xTr[i], xTr[j] );
				else
					sum += 10.0 * max(0.0, 1-distance2( xTr[i], xTr[j] ) );
			}
		}
		return sum;
	}

	vector< vector<double> > train
		( vector< vector<double> >xTr , vector<int>yTr, vector< vector<double> >L=vector< vector<double> >(),
		int d=3, int itermax = -1){

		int Kg = 5;
		double step = 1e-5;
		int N = xTr.size();
		int D = xTr[0].size();

		if ( L.empty() ){
			L = vector< vector<double> >(d, vector<double>(D) );
			for (int i=L.size()-1; i>=0; i--){
				for (int j=L[i].size()-1; j>=0; j--){
					L[i][j] = (double)rand();
					//L[i][j] = 0;
				}
				//L[i][i] = 1;
			}
		}
		if ( itermax<0 ) itermax = xTr.size();

		

		double nm = norm(L);

		for (int i=L.size()-1; i>=0; i--){
			for (int j=L[i].size()-1; j>=0; j--){
				L[i][j] /= nm;
			}
		}

		int iter = 0;
		while( iter++<itermax ){
			double h = getH1(xTr, yTr, L);

			
			//cout<<"h: "<<h<<" norm: "<<norm(L)<<endl;
			//retrieval_test(xTr, yTr, xTe, yTe, L );

			auto grad = getGrad1(xTr, yTr, L);
			double step = 1e-5;
			{
				for (int i=0; i<d; i++){
					for (int j=0; j<D; j++){
						L[i][j] -= grad[i][j] * step;					
					}
				}
			}
		}
		return L;
	}

};