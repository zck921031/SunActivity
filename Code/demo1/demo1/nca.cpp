#include "nca.h"

namespace nca_debug{
	//O(nd)
	double distance2(const vector<double>&x, const vector<double>&y, const vector< vector<double> > &A){
		assert(  x.size() == y.size() && x.size()>0 && A.size()>0 && x.size()==A[0].size() );
		int d = A.size();
		int n = A[0].size();
		double sum = 0;
		vector<double> u(n);
		for (int i=0; i<n; i++){
			u[i] = x[i]-y[i];
		}
		
		for (int i=0; i<d; i++){
			double tmp = 0;
			for (int j=0; j<n; j++){
				tmp += A[i][j] * u[j];
			}
			sum += tmp*tmp;
		}
		return sum;
	}
	void addby(vector<double>&x, double c, const vector<double>&y){
		for ( int i=x.size()-1; i>=0; i-- ){
			x[i] += c*y[i];
		}
	}
	void addby(matrix2d &x, double c, const matrix2d &y){
		for ( int i=x.size()-1; i>=0; i-- ){
			for ( int j=x[i].size()-1; j>=0; j--){
				x[i][j] += c*y[i][j];
			}
		}
	}
	void multiby(matrix2d &x, double c, const matrix2d &y){
		int n = x.size();
		int m = x[0].size();
		int l = y[0].size();
		assert(m == y.size() );

		matrix2d t = matrix2d(n, vector<double>(m,0) );
		for (int i=0; i<n; i++){
			for (int j=0; j<m; j++){
				for (int k=0; k<l; k++)
					t[i][k] += x[i][j] * y[j][k];
			}
		}
		x  = t;
	}
	double get_F(const matrix2d&x, const vector<int>&label, const matrix2d&A){
		int N  = x.size();
		double F = 0;
		vector<double> s(N,0);
		for (int i=0; i<N; i++){
			double fm = 0;
			for (int j=0; j<N; j++){
				s[j] = exp( -distance2(x[i], x[j], A) );
				if( i==j ) s[j] = 0;
				fm += s[j];
			}
			for (int j=0; j<N; j++){
				if ( label[i] == label[j] ) {
					F += s[j] / fm;
				}
			}
		}
		if ( F!=F ) F = -1;
		return F;
	}
	double test(const matrix2d&x, const vector<int>&label, const matrix2d&A){		
		int N = x.size();
		vector<double> s(N,0);
		int right = 0;
		for (int i=0; i<N; i++){
			vector<double> score(N,0);
			double fm = 0;
			for (int j=0; j<N; j++){
				s[j] = exp( -distance2(x[i], x[j], A) );
				score[ label[j] ] += s[j];
			}
			int c = max_element( score.begin(), score.end() ) - score.begin();
			if ( c == label[i] ) right++;
		}
		return (double)right/N;
	}
}
using namespace nca_debug;

matrix2d nca_solve(const matrix2d&x, const vector<int>&label, int d, int iter, double step )
{
	assert( x.size() == label.size() );
	assert( d>=1 && iter>=1 );

	int N = x.size();
	int D = x[0].size();
	vector< vector<double> > A = vector< vector<double> >(d, vector<double>(D,0) );
	for ( int i=0; i<min(d,D); i++ ) A[i][i] = 0.03;

	while ( iter-- > 0 ){
		double F = 0;
		vector<double> P(N,0);
		matrix2d p = matrix2d(N, vector<double>(N,0) );
		for (int i=0; i<N; i++){
			vector<double> s(N,0);
			double fm = 0;
			for (int j=0; j<N; j++){
				s[j] = exp( -distance2(x[i], x[j], A) );
				if( i==j ) s[j] = 0;
				fm += s[j];
			}
			for (int j=0; j<N; j++){
				p[i][j] = s[j] / fm;
				if ( label[i] == label[j] ) {
					P[i] += p[i][j];
					F += p[i][j];
				}
			}
			//cout<<i<<" : "<<fm<<endl;
		}
	
	//	for (int i=0; i<N; i++){
	//		cout<<"P["<<i<<"] = "<<P[i]<<" label: "<<label[i]<<endl;
	//	}
	
		cout<<"F: "<<F<<endl;	//checked!
		cout<<"test: "<<test(x, label, A)<<endl;
		
		//求出导数
		matrix2d G = matrix2d(d, vector<double>(D,0) );
		matrix2d t = matrix2d(d, vector<double>(D,0) );
		vector<double> u(D,0);
		for (int i=0; i<N; i++){
			for (int k=0; k<N; k++){
				for (int a=0; a<D; a++)	u[a] = x[i][a] - x[k][a];

				for (int a=0; a<D; a++){
					for (int b=0; b<D; b++){
						t[a][b] = u[a]*u[b];
					}
				}
				addby(G, P[i]*p[i][k], t);
			
				if (  label[i] == label[k] ){				
					addby(G, -p[i][k], t);
				}
			}
		}
		multiby(G, 2, A);
		/*
		for (int i=0; i<d; i++){
			for (int j=0; j<d; j++){
				cout<<G[i][j]<<" ";
			}
			cout<<endl;
		}
		*/

		matrix2d A_tmp=A, A_best=A;
		double F_best = 0;
		/*
		while ( (F_tmp=get_F(x, label, A_tmp)<0) ){
			for (int i=0; i<d; i++){
				for (int j=0; j<D; j++){ 
					A_tmp[i][j] *= 0.8;
				}				
			}
			cout<<"F_tmp: "<<F_tmp<<" .. ";
		}
		*/
		
		for ( double step = 1; step > 1e-8; step *= 0.5 ){			
			for (int i=0; i<d; i++){
				for (int j=0; j<D; j++){
					A_tmp[i][j] = A[i][j] + step * G[i][j];
				}
			}
			double F_tmp=get_F(x, label, A_tmp);
			if ( F_tmp > F_best ) {
				F_best = F_tmp;
				A_best = A_tmp;
			}
		}				

		if ( F_best > F ){
			A = A_best;
			F = F_best;
			cout<<"F_new: "<<F<<endl;		
			cout<<"test: "<<test(x, label, A)<<endl;
		}else{
			break;
		}
	}
	return A;
}

void wine_demo(){
	FILE *fin = fopen("..//..//..//data//wine//wine_train.data","r");
	if ( fin == NULL ) return ;
	matrix2d x;
	vector<int> label;
	int c;
	int D = 13;
	while ( fscanf(fin, "%d", &c) != EOF ){
		vector<double> elem(D);
		for (int i=0; i<D; i++){
			fscanf(fin, ",%lf", &elem[i]);
		}
		label.push_back(c);
		x.push_back(elem);
	}
	fclose(fin);

//	for (int i=0; i<(int)x.size(); i++){
//		for ( auto t:x[i] ){
//			cout<<t<<" ";
//		}
//		cout<<endl;
//	}

	nca_solve(x, label, D, 30, 1);
}
