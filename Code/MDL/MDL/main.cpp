#include "func.h"

#include "stdafx.h"


void MDL();

int main(){
	
	MDL();

	system("Pause");

}


/**
 * contraint (x_i, x_j, x_k) : d(x_i, x_k) - d(x_i, x_j) >= 1
 * hinge loss: max(0, 1 + d2(x_i, x_j) - d2(x_i, x_k) ) s.t. (i,j,k) \in R
 * regular: d2(x_i, x_j) s.t. (i,j) \in S
 */
double getH(vector< vector<double> > xTr, vector<int> yTr, const vector< vector<double> > &L){	

	int N=xTr.size(), D=xTr[0].size(), d=L.size();

	for ( auto &t : xTr ){
		t = project(t, L);
	}

	double sum = 0;

	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			if ( yTr[i] == yTr[j] )
				sum += distance2( xTr[i], xTr[j] );
		}
	}

	
	vector< vector<double> > dist(N, vector<double>(N,0) );
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			dist[i][j] = distance2(xTr[i], xTr[j]);
		}
	}

	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			if ( yTr[i] != yTr[j] ) continue;
			for (int k=0; k<N; k++){
				if ( yTr[i] == yTr[k] ) continue;
				sum += max(0.0,  1 + dist[i][j] - dist[i][k] );
			}
		}
	}
	
	return sum;
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

vector< vector<double> > getGrad1( vector< vector<double> > xTr, vector<int> yTr, const vector< vector<double> > &L )
{
	int d = L.size(), D=L[0].size(), N=xTr.size();
	vector< vector<double> > g(d, vector<double>(D,0) );
	//vector< vector<double> > g = L;
	

	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
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

vector< vector<double> > getGrad( vector< vector<double> > xTr, vector<int> yTr, const vector< vector<double> > &L )
{
	int d = L.size(), D=L[0].size(), N=xTr.size();
	//vector< vector<double> > g(d, vector<double>(D,0) );
	vector< vector<double> > g = L;
	

	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
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
						g[u][v] += tmp[u] * x[v];
					}
				}
			}
		}
	}

	vector< vector<double> > dist(N, vector<double>(N,0) );
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			dist[i][j] = distance2( project(xTr[i], L), project(xTr[j],L) );
		}
	}
	
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			if ( yTr[i] != yTr[j] ) continue;
			for (int k=0; k<N; k++){
				if ( yTr[i] == yTr[k] ) continue;
				if ( 1 + dist[i][j] - dist[i][k] <= 0 ) continue;
				//if ( 1 + distance2(xTr[i], xTr[j]) - distance2(xTr[i], xTr[k] ) <= 0 ) continue;

				vector<double> x(D, 0);
				for (int u=0; u<D; u++){
					x[u] = xTr[i][u] - xTr[j][u];
				}
				vector<double> tmp(d, 0);
				for (int u=0; u<d; u++){
					for (int v=0; v<D; v++){
						tmp[u] += L[u][v]*x[v];
					}
				}
				for (int u=0; u<d; u++){
					for (int v=0; v<D; v++){
						g[u][v] += tmp[u] * x[v];
					}
				}

				x=vector<double>(D, 0);
				for (int u=0; u<D; u++){
					x[u] = xTr[i][u] - xTr[k][u];
				}
				tmp=vector<double>(d, 0);
				for (int u=0; u<d; u++){
					for (int v=0; v<D; v++){
						tmp[u] += L[u][v]*x[v];
					}
				}
				for (int u=0; u<d; u++){
					for (int v=0; v<D; v++){
						g[u][v] -= tmp[u] * x[v];
					}
				}

			}
		}
	}

	return g;
}

double norm(const vector< vector<double> > & g){
	double sum = 0;
	for (auto &i : g){
		for (auto &t : i){
			sum = sum + t*t;
		}
	}
	return sqrt(sum);
}
void MDL(){
	vector< vector<double> > xTr, xTe;
	vector<int> yTr, yTe;
	load_feature( RealPath("C://Users//zck//Documents//GitHub//SunActivity//data//TrainSet//feature//"),
		xTr, yTr, xTe, yTe, 0.5);
	

	if ( xTr.empty() ) return ;
	cout<<xTr.size()<<" "<<xTr[0].size()<<" "<<xTe.size()<<" "<<xTe[0].size()<<endl;

	int D = xTr[0].size();
	retrieval_test(xTr, yTr, xTe, yTe, vector< vector<double> >() );

	cout<<endl;

	//xTr.erase( xTr.begin()+10, xTr.end() );
	int d = 3;
	vector< vector<double> > L(d, vector<double>(D) );
	for (int i=L.size()-1; i>=0; i--){
		for (int j=L[i].size()-1; j>=0; j--){
			L[i][j] = (double)rand();
			//L[i][j] = 0;
		}
		//L[i][i] = 1;
	}	
	double nm = norm(L);
	nm = 100;
	for (int i=L.size()-1; i>=0; i--){
		for (int j=L[i].size()-1; j>=0; j--){
			L[i][j] /= nm;
		}
	}

	int iter = 0;
	while( iter++<300 ){
		double h = getH1(xTr, yTr, L);

		
		cout<<"h: "<<h<<" norm: "<<norm(L)<<endl;
		retrieval_test(xTr, yTr, xTe, yTe, L );

		auto grad = getGrad1(xTr, yTr, L);
		//for ( double step = 1e-12; step<1; step*=2 )
		double step = 1e-6;
		{
			//auto L_tmp = L;
			for (int i=0; i<d; i++){
				for (int j=0; j<D; j++){
					L[i][j] -= grad[i][j] * step;					
				}
			}
			//double h_tmp = getH(xTr, yTr, L_tmp);

			//if ( h_tmp < h_best ){
				//h_best = h_tmp;
				//L_best = L_tmp;
				//step *= 1.05;
			//}
		}

		//if ( L == L_best ){
			//cout<<"no longer working"<<endl;
			//break;
		//}
		//L = L_best;

	}

	
	retrieval_test(xTr, yTr, xTr, yTr, L );
	retrieval_test(xTr, yTr, xTe, yTe, L );

}