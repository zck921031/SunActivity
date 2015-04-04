#pragma once

#include "nca.h"

namespace nca_debug{
	
	//O(D)
	double distance2(const vector<double>&x, const vector<double>&y){
		double sum = 0;
		for (int i=x.size()-1; i>=0; i--){
			sum += (x[i]-y[i])*(x[i]-y[i]);
		}
		return sum;
	}
	//O(Dd)
	double distance2(const vector<double>&x, const vector<double>&y, const vector< vector<double> > &A){
		assert(  x.size() == y.size() && x.size()>0 && A.size()>0 && x.size()==A[0].size() );
		int d = A.size();
		int D = A[0].size();
		double sum = 0;
		vector<double> u(D);
		for (int i=0; i<D; i++){
			u[i] = x[i]-y[i];
		}
		for (int i=0; i<d; i++){
			double tmp = 0;
			for (int j=0; j<D; j++){
				tmp += A[i][j] * u[j];
			}
			sum += tmp*tmp;
		}
		return sum;
	}
	void addby(vector<double>&x, double c, const vector<double>&y){
		assert( x.size()==y.size() );
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
	double getF(const matrix2d&x, const vector<int>&label, const matrix2d&A){
		cout<<"Enter getF, this is a slower method."<<endl;
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

	//project X to an optimized space: X := AX, here X \in mathbb{R}^{DxN} while in the program X is D cols with N rows.
	matrix2d project(const matrix2d&x_origin, const matrix2d&A ){		
		int N = x_origin.size();
		int D = x_origin[0].size();
		int d = A.size();
		matrix2d x = matrix2d(N, vector<double>(d,0) );
		for (int i=0; i<N; i++){
			for (int a=0; a<d; a++){
				for (int b=0; b<D; b++){
					x[i][a] += A[a][b] * x_origin[i][b];
				}
			}
		}
		return x;
	}

	double getF_fast(const matrix2d&x_origin, const vector<int>&label, const matrix2d&A, matrix2d &p, vector<double> &P){
		matrix2d x;
		x = project(x_origin, A);
		//x = x_origin;
		int N = x.size();
		p = matrix2d(N, vector<double>(N,0) );
		P = vector<double>(N,0);
		double F = 0;
		vector<double> s(N,0);
		for (int i=0; i<N; i++){
			double fm = 0;
			for (int j=0; j<N; j++){
				s[j] = exp( -distance2(x[i], x[j]) );
				if( i==j ) s[j] = 0;
				fm += s[j];
			}
			for (int j=0; j<N; j++){
				p[i][j] = s[j] / fm;
				if ( label[i] == label[j] ) {
					P[i] += p[i][j];
					///if( label[i]>0 )
					F += p[i][j];
				}
			}
		}
		if ( F!=F ) F = -1;
		//cout<<distance2(x[0], x[1])<<" "<<distance2(x[0], x[1], A)<<" F:"<<F<<endl;
		//exit(0);
		return F;
	}

	int classify_1(const matrix2d&train_x, const vector<int>&train_label, const vector<double>&x, const matrix2d&A){
		int N = train_x.size();
		vector<double> score(N,0);
		for (int i=0; i<N; i++){
			score[ train_label[i] ] += exp( -distance2(x, train_x[i], A) );
		}
		return max_element( score.begin(), score.end() ) - score.begin();
	}	
	int classify_knn(const matrix2d&train_x, const vector<int>&train_label, const vector<double>&x, const matrix2d&A ){
		bool useEuclidean = A.empty();
		int N = train_x.size();
		vector< pair<double,int> > dist(N);
		for (int i=0; i<N; i++){
			if (useEuclidean){
				dist[i].first = distance2(x, train_x[i]);
			}else{
				dist[i].first = distance2(x, train_x[i], A);
			}
			dist[i].second = train_label[i];
		}
		sort( begin(dist), end(dist) );
		int K = 3;
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


	double test(const matrix2d&train_x_origin, const vector<int>&train_label,
		const matrix2d&test_x_origin, const vector<int>&test_label, const matrix2d&A){

		map< int, map<int,int> > mat;

		matrix2d train_x = project(train_x_origin, A);
		matrix2d test_x = project(test_x_origin, A);
		int right = 0;
		int M = test_x.size();
		for (int i=0; i<M; i++){
			int res = classify_knn(train_x, train_label, test_x[i] );
			if ( res == test_label[i] ) right++;
			mat[ test_label[i] ][ res ]++;
		}

		if( train_x_origin.size() != test_x_origin.size() ){
			cout<<endl;
			for ( int i=0; i<4; i++ ){
				for ( int j=0; j<4; j++ ){
					cout<<mat[i][j]<<" ";
				}
				cout<<endl;
			}
			cout<<endl;
		}

		return (double)right/M;
	}

}
using namespace nca_debug;

matrix2d Gradient1(const matrix2d&x, const vector<int>&label, const matrix2d&A, const matrix2d &p, const vector<double> &P ){
	cout<<"Enter Gradient1, this is a slower method."<<endl;
	int d = A.size();
	int D = A[0].size();
	int N = x.size();
	matrix2d G = matrix2d(d, vector<double>(D,0) );
	matrix2d t = matrix2d(d, vector<double>(D,0) );
	vector<double> u(D,0);
	for (int i=0; i<N; i++){
		for (int k=0; k<N; k++){
			for (int a=0; a<D; a++)	u[a] = x[i][a] - x[k][a];
			for (int a=0; a<d; a++){
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
	return G;
}

matrix2d Gradient1_fast(const matrix2d&x, const vector<int>&label, const matrix2d&A, const matrix2d &p, const vector<double> &P){
	int d = A.size();
	int D = A[0].size();
	int N = x.size();
	matrix2d C = matrix2d(N, vector<double>(N,0) );
	for (int i=0; i<N; i++){
		for (int j=0; j<N; j++){
			double cov = P[i]*p[i][j];			
			if (  label[i] == label[j] )
			///if ( label[i]>0 )
			{
				cov -= p[i][j];
			}
			C[i][i] += cov;	C[j][j] += cov;
			C[i][j] -= cov;	C[j][i] -= cov;
		}
	}
	
	matrix2d G  = matrix2d(d, vector<double>(D,0) );
	matrix2d Gi = matrix2d(d, vector<double>(D,0) );
	
	for (int i=0; i<N; i++){
		vector<double> t(D,0);
		for (int j=0; j<N; j++){
			addby(t, C[i][j], x[j]);
		}
		for (int j=0; j<D; j++){
			for (int k=0; k<D; k++){
				Gi[j][k] = x[i][j]*t[k];
			}
		}
		addby(G, 1.0, Gi);
	}
	multiby(G, 2, A);
	return G;
}


matrix2d nca_solve1(const matrix2d&x, const vector<int>&label, int d, int iter, double step )
{
	assert( x.size() == label.size() );
	assert( d>=1 && iter>=1 );
	
	int N = x.size();
	int D = x[0].size();
	vector< vector<double> > A = vector< vector<double> >(d, vector<double>(D,0) );
	for ( int i=0; i<min(d,D); i++ ) A[i][i] = 1.0/D;

	vector<double> P(N,0);
	matrix2d p = matrix2d(N, vector<double>(N,0) );
	while ( iter-- > 0 ){
		double F = 0;
		F = getF_fast(x, label, A, p, P);
	//	for (int i=0; i<N; i++){
	//		cout<<"P["<<i<<"] = "<<P[i]<<" label: "<<label[i]<<endl;
	//	}	
		cout<<"F: "<<F<<" ";	//checked!
		cout<<"test: "<<test(x, label, x, label, A)<<".  ";
		//计算梯度G
		//matrix2d G = Gradient1(x, label, A, p, P);
		matrix2d G = Gradient1_fast(x, label, A, p, P);
	
//		for (int i=0; i<d; i++){
//			for (int j=0; j<d; j++){
//				cout<<G[i][j]<<" ";
//			}
//			cout<<endl;
//		}

		matrix2d A_tmp=A, A_best=A;
		double F_best = 0;
		//避免出现数值运算问题
//		while ( (F_tmp=getF_fast(x, label, A, p, P)<0) ){
//			for (int i=0; i<d; i++){
//				for (int j=0; j<D; j++){ 
//					A_tmp[i][j] *= 0.8;
//				}
//			}
//			cout<<"F_tmp: "<<F_tmp<<" .. ";
//		}
				
		for ( double step = 1; step > 1e-8; step *= 0.5 ){			
			for (int i=0; i<d; i++){
				for (int j=0; j<D; j++){
					A_tmp[i][j] = A[i][j] + step * G[i][j];
				}
			}
			double F_tmp=getF_fast(x, label, A_tmp, p, P);
			if ( F_tmp > F_best ) {
				F_best = F_tmp;
				A_best = A_tmp;
			}
		}

		if ( F_best > F+1e-5 ){
			A = A_best;
			F = F_best;
			cout<<"F_new: "<<F<<" ";		
			cout<<"test: "<<test(x, label, x, label, A)<<endl;
		}else{
			break;
		}

		//cout<<"Press Y for continue."<<endl;
		//char buf[1024];
		//cin.getline(buf, 1024);
		//if ( !(*buf=='Y' || *buf=='y') ) break;
	}
	return A;
}



void wine_demo(){
	//read train data
	FILE *fin = fopen("..//..//..//data//wine//wine_train.data","r");
	if ( fin == NULL ) return ;
	matrix2d train_x;
	vector<int> train_label;
	int c;
	int D = 13;
	while ( fscanf(fin, "%d", &c) != EOF ){
		vector<double> elem(D);
		for (int i=0; i<D; i++){
			fscanf(fin, ",%lf", &elem[i]);
		}
		train_label.push_back(c);
		train_x.push_back(elem);
	}
	fclose(fin);
	//read test data
	fin = fopen("..//..//..//data//wine//wine_test.data","r");
	if ( fin == NULL ) return ;
	matrix2d test_x;
	vector<int> test_label;
	while ( fscanf(fin, "%d", &c) != EOF ){
		vector<double> elem(D);
		for (int i=0; i<D; i++){
			fscanf(fin, ",%lf", &elem[i]);
		}
		test_label.push_back(c);
		test_x.push_back(elem);
	}
	fclose(fin);
	//run testing using Euclidean
	matrix2d A = matrix2d(D, vector<double>(D,0));
	for (int i=0; i<D; i++) A[i][i] = 1;
	cout<<"before dml, test is: "<<test(train_x, train_label, test_x, test_label, A)<<endl;
	//distance metric learning
	A = nca_solve(train_x, train_label, D, 30, 1);	
	//run testing using A
	cout<<"after dml, test is: "<<test(train_x, train_label, test_x, test_label, A)<<endl;

}



matrix2d nca_solve(const matrix2d&x, const vector<int>&label, int d, int iter, double step ){
	return nca_solve1(x, label, d, iter, step );
}
