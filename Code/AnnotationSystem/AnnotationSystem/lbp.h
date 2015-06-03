
/**
 * 计算图片的lbp二值化模式
 * API: vector<double> res = Lbp59(Mat gray_image);
 * input: Mat gray_image
 * output: 归一化二值化模式统计直方图，59维。
 */

template <typename _Tp> vector<double> Lbp256(Mat& src, Mat &dst = Mat( Size(0,0), CV_8UC1), bool debug = false  );
template <typename _Tp> vector<double> Lbp59(Mat& src, Mat &dst = Mat( Size(0,0), CV_8UC1), bool debug = false );

namespace __LBP{
	bool ISINIT = false;
	unsigned char table[256];
	int getHopCount(uchar i)
	{
		int a[8]={0};
		int k=7;
		int cnt=0;
		while(i)
		{
			a[k]=i&1;
			i>>=1;
			--k;
		}
		for(k=0; k<8; ++k)
		{
			if(a[k]!=a[k+1==8?0:k+1])
			{
				++cnt;
			}
		}
		return cnt;
	}
	void lbp59table()
	{
		memset(table, -1, sizeof(table) );
		uchar temp=0;
		for(int i=0; i<256; i++)
		{
			if(getHopCount(i)<=2)
			{
				table[i]=temp;
				temp++;
			}
		}
		for (int i=0; i<256; i++){
			if ( table[i]==255 ){
				table[i] = temp;
			}			
			//printf("%d\n",table[i]);
		}
		//printf("total lbp = %d\n", temp+1);
	}

	void Lbp_init(){
		lbp59table();
		ISINIT = true;
	}

	void test(){		
		Mat gray;
		gray = load_image("C://Users//zck//Documents//GitHub//selfwork//东南大学//毕业设计//毕业设计论文//图表//颜色特征//rgb.jpg");
		Mat dst;

		auto a = Lbp256<uchar>(gray, dst, true);
		//auto a = Lbp59<uchar>(gray);
		ofstream os("C://Users//zck//Documents//GitHub//selfwork//东南大学//毕业设计//毕业设计论文//图表//LBP特征//hist.txt");
		for (auto t : a) os<<t<<" ";cout<<endl;
		os.close();
		
		imshow("1", gray);
		imshow("2", dst);
		waitKey();
		imwrite("C://Users//zck//Documents//GitHub//selfwork//东南大学//毕业设计//毕业设计论文//图表//LBP特征//lbp.jpg", dst);
	}
}

template <typename _Tp>
vector<double> Lbp256(Mat& src, Mat &dst, bool debug) {
	if ( ! __LBP::ISINIT ) __LBP::Lbp_init();

	vector<double> hist(256, 0);
	if (debug){
		// allocate memory for result
		dst = Mat( Size(src.cols-2, src.rows-2 ), CV_8UC1);
		// zero the result matrix
		dst.setTo(0);
		cout<<"LBP: debug mode."<<endl;
	}

    // calculate patterns
    for(int i=1;i<src.rows-1;i++) {
        for(int j=1;j<src.cols-1;j++) {			
            _Tp center = src.at<_Tp>(i,j);
			//cout<<"center"<<(int)center<<"  ";
            unsigned char code = 0;
            code |= (src.at<_Tp>(i-1,j-1) >= center) << 7;
            code |= (src.at<_Tp>(i-1,j  ) >= center) << 6;
            code |= (src.at<_Tp>(i-1,j+1) >= center) << 5;
            code |= (src.at<_Tp>(i  ,j+1) >= center) << 4;
            code |= (src.at<_Tp>(i+1,j+1) >= center) << 3;
            code |= (src.at<_Tp>(i+1,j  ) >= center) << 2;
            code |= (src.at<_Tp>(i+1,j-1) >= center) << 1;
            code |= (src.at<_Tp>(i  ,j-1) >= center) << 0;

            if (debug){
				dst.at<unsigned char>(i-1,j-1) = code;
			}
			
			hist[code]++;
        }
    }
	return hist;
}


template <typename _Tp>
vector<double> Lbp59(Mat& src, Mat &dst, bool debug) {
	vector<double> hist(59, 0);

	vector<double> res = Lbp256<_Tp>(src, dst, debug);
	for (int i=0; i<256; i++){
		hist[ __LBP::table[i] ] += res[i];
	}
	if (debug){
		for (int i=0; i<dst.rows; i++)
			for (int j=0; j<dst.cols; j++){
				dst.at<_Tp>(i,j) = __LBP::table[ dst.at<_Tp>(i,j) ];
			}
	}

	double sum = 0;
	for ( auto &t : hist ){
		sum = sum + t*t;
	}
	sum = sqrt(sum);
	if ( sum<1 ) sum = 1;
	for ( auto &t : hist ){
		t = t/sum;
	}
	return hist;
}

