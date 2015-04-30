
#include "func.h"

/**
 * 从文件中读取模板数据的特征
 * input: path 放特征的文件夹; ratio 用作训练集的数据比例
 * output: x 一行一个特征; y 一行一个标记
 */

void load_feature( string path, vector< vector<double> >&xTr , vector<int>&yTr,
				  vector< vector<double> >&xTe, vector<int>&yTe, double ratio ){
	vector< vector<double> > xtmp;
	vector<int> ytmp;
	map<int,int> ycnt;
	ifstream is(path + "//concept.txt");
	{
		int t;
		while( is>>t ){
			ytmp.push_back(t);
			ycnt[t]++;
		}
	}
	for ( auto &it : ycnt ){
		it.second = (int)ceil( it.second * ratio );
	}
	int N = ytmp.size();
	xtmp = vector< vector<double> >(N);
	vector<bool> skip(N, 0);
	for (int i=0; i<N; i++){
		if ( ycnt[ ytmp[i] ]-- <= 0 ) skip[i] = true;
		else skip[i] = false;
	}


	for ( string waveName : modal )
	for ( string ftype : featureType )
	{
		string filename = path + "//" + waveName +  "." + ftype + ".txt";
		vector< vector<double> > g = csvread( filename );
		assert( N == g.size() );
		for (int i=0; i<N; i++){
			for ( auto t : g[i] ){
				xtmp[i].push_back(t);
			}
		}
	}

	xTr.clear(), yTr.clear();
	xTe.clear(), yTe.clear();
	for (int i=0; i<N; i++){
		if ( !skip[i] ){
			xTr.push_back( xtmp[i] );
			yTr.push_back( ytmp[i] );
		}else{
			xTe.push_back( xtmp[i] );
			yTe.push_back( ytmp[i] );
		}
	}

}



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


string RealPath(string prefix){
	return prefix + "//" + Concept+"_No"+Concept + "//feature//";
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

//Recognition(string Concept, const map<string,string>&_pars, int size=320, int board=1500):
//	Concept(Concept),pars(_pars),size(size),board(board)
//{
//	string realpath = RealPath( pars["featurepath"] );
//
//	load_feature( realpath, xTr, yTr, sift_bows, 0.5 );
//
//	L.clear();
//	if ( "Euclidean" != pars["distanceName"] ){
//		L = csvread(realpath + "//" + pars["distanceName"] );
//		for (int i=xTr.size()-1; i>=0; i--){
//			xTr[i] = project(xTr[i], L);
//		}
//	}
//
//
//}
	
	

double distance2(const vector<double>&x, const vector<double>&y){
	double sum = 0;
	for (int i=x.size()-1; i>=0; i--){
		sum += (x[i]-y[i])*(x[i]-y[i]);
	}
	return sum;
}

int classify(vector< vector<double> >xTr , vector<int>yTr,
				  vector<double>xTe, const vector< vector<double> >&L, bool isprojected){
	
	if ( !isprojected ){
		for ( auto &x : xTr){
			x = project(x, L);
		}
		xTe = project(xTe, L);
	}	

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
		//if ( i==1 ) cerr<<dis<<endl;

	}
	//cerr<<sump<<" , "<<sumn<<endl;

	sump /= cntp;
	sumn /= cntn;
	return sump < 1.0*sumn ? 1 : -1;
}


int classify_knn(vector< vector<double> >xTr , vector<int>yTr, const vector<double>&xTe){	
	int N = xTr.size();
	vector< pair<double,int> > dist(N);
	for (int i=0; i<N; i++){
		dist[i].first = distance2(xTe, xTr[i]);
		dist[i].second = yTr[i];
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

void retrieval_test(vector< vector<double> >xTr , vector<int>yTr,
				  vector< vector<double> >xTe , vector<int>yTe, const vector< vector<double> >&L){
	

	for ( auto &x : xTr){ x = project(x, L); }
	for ( auto &x : xTe){ x = project(x, L); }
	

	double TP=0, FN=0, FP=0, TN=0, T=0, F=0;
	for (int i = xTe.size()-1; i>=0; i--){
		if ( classify(xTr, yTr, xTe[i], L, true ) == yTe[i] ){
			T += 1;
		}else{
			F += 1;
		}
	}
	cout<<"类平均法正确率: "<<T/(F+T)*100<<"%"<<endl;

	T=F=0;
	for (int i = xTe.size()-1; i>=0; i--){
		if ( classify_knn(xTr, yTr, xTe[i] ) == yTe[i] ){
			T += 1;
		}else{
			F += 1;
		}
	}
	cout<<"5-NN正确率: "<<T/(F+T)*100<<"%"<<endl;

	double sumP=0, sumN=0;
	for (int i=xTr.size()-1; i>=0; i--){
		for (int j=xTr.size()-1; j>=0; j--){
			if ( yTr[i] == yTr[j] ){
				sumP += distance2(xTr[i], xTr[j]);
			}else{
				sumN += distance2(xTr[i], xTr[j]);
			}
		}
	}
	
	cout<<"训练集相关/非相关距离平方和: "<<sumP<<" , "<<sumN<<endl;
	
	sumP=sumN=0;
	for (int i=xTe.size()-1; i>=0; i--){
		for (int j=xTe.size()-1; j>=0; j--){
			if ( yTr[i] == yTr[j] ){
				sumP += distance2(xTe[i], xTe[j]);
			}else{
				sumN += distance2(xTe[i], xTe[j]);
			}
		}
	}
	cout<<"测试集相关/非相关距离平方和: "<<sumP<<" , "<<sumN<<endl;

}