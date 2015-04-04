#include "feature.h"

vector< vector<uchar> > read_jpg_to_vector(const char* filename)
{
	Mat img=imread(filename);

	Mat gray;
	cvtColor(img, gray ,CV_BGR2GRAY);

	int rows = gray.rows;
	int cols = gray.cols;
	vector< vector<uchar> > ret = vector< vector<uchar> >(rows, vector<uchar>(cols, 0) );
	for (int i=0; i<rows; i++ )
		for (int j=0; j<cols; j++)
		{
			ret[i][j] = gray.at<uchar>(i,j);
		}
	return ret;
}


Mat read_jpg_to_gray_mat(const char* filename)
{
	Mat img=imread(filename);

	//namedWindow("1");
	//imshow("1",img);

	Mat ret;
	cvtColor(img, ret ,CV_BGR2GRAY);

	//namedWindow("2");
	//imshow("2",ret);

	return ret;
}

vector<double> get_Hist_from_mat(const Mat &img){
	vector<double>cnt(256,0);
	double sum = 0;
	for (int i=0; i<img.cols; i++){
		for (int j=0; j<img.rows; j++){
			int k = img.at<uchar>(j, i);
			cnt[k] += 1.0;
		}
	}
	for (int i=0; i<256; i++){
		sum += cnt[i];
	}
	for (int i=0; i<256; i++){
		cnt[i] = cnt[i] / sum * 256;
	}
	return cnt;
}

vector<double> get_ColorMoment_from_mat(const Mat &img)
{
	vector<double> cm(3,0);
	//一阶矩mu
	double mu = 0;
	for (int i=0; i<img.rows; i++)
		for (int j=0; j<img.cols; j++)
		{
			mu += img.at<uchar>(i,j);
		}
	mu /= img.rows*img.cols;

	//二阶矩sigma和三阶矩s
	double sigma=0, s=0;
	for (int i=0; i<img.rows; i++)
		for (int j=0; j<img.cols; j++)
		{
			double tmp = img.at<uchar>(i,j)-mu;
			sigma += tmp*tmp;
			s += tmp*tmp*tmp;
		}
	sigma = sqrt( sigma/ (img.rows*img.cols) );
	s /= (img.rows*img.cols);
	if ( s>0 )
		s = exp( 1.0/3.0 * log(s) );
	else
		s = -exp( 1.0/3.0 * log(-s) );

	cm[0] = mu;
	cm[1] = sigma;
	cm[2] = s;
	return cm;
}


vector<double> get_texture_from_mat(const Mat &img, int N)
{
	int NN = 256/N;
	vector<double> ret;
	int rows = img.rows, cols = img.cols;
	vector< vector<uchar> > g = vector< vector<uchar> >(rows, vector<uchar>(cols, 0) );
	//灰度图量化为N阶。这里的量化方式和Matlab略有不同。
	for (int i=0; i<rows; i++)
	for (int j=0; j<cols; j++)
	{
		g[i][j] = img.at<uchar>(i,j) / NN;
	}

	
	int dx[] = { 0, -1, -1, -1};
	int dy[] = { 1,  1,  0, -1};
	
	for (int c=0; c<4; c++)
	{
		double Contrast=0, Correlation=0, Energy=0, Homogeneity=0;
		vector< vector<double> > G = vector< vector<double> >(N, vector<double>(N, 0) );		

		double Sum = 0;
		for (int i=0; i<rows; i++)
		for (int j=0; j<cols; j++)
		{
			int x=i+dx[c], y=j+dy[c];
			if (x<0 || x>=rows || y<0 || y>=cols) continue;
			G[ g[i][j] ][ g[x][y] ] += 1;
			Sum += 1;
		}
		//归一化灰度共生矩阵
		for (int i=0; i<N; i++)
		for (int j=0; j<N; j++)
		{
			G[i][j] /= Sum;
		}

		for (int i=0; i<N; i++)
		for (int j=0; j<N; j++)
		{
			Energy += G[i][j]*G[i][j];
			Contrast += (i-j)*(i-j)*G[i][j];
			Homogeneity += G[i][j]/(double)(1 + (i-j)*(i-j) );
		}
		
		double ui=0, uj=0, si2=0, sj2=0;
		for (int i=0; i<N; i++)
		for (int j=0; j<N; j++)
		{
			ui += G[i][j]*(i+1);
			uj += G[i][j]*(j+1);
		}
		for (int i=0; i<N; i++)
		for (int j=0; j<N; j++)
		{
			si2 += G[i][j]*(i+1-ui)*(i+1-ui);
			sj2 += G[i][j]*(j+1-uj)*(j+1-uj);
			Correlation += (i+1)*(j+1)*G[i][j];
		}
		Correlation = (Correlation-ui*uj)/sqrt(si2*sj2);	

		//cout<<"Contrast: "<<Contrast<<" Correlation: "<<Correlation<<" Energy: "<<Energy<<" Homogeneity: "<<Homogeneity<<endl;
		ret.push_back(Contrast);
		ret.push_back(Correlation);
		ret.push_back(Energy);
		ret.push_back(Homogeneity);
	}

	return ret;
}