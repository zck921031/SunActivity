#pragma once

#include"demo1.h"
#include"feature.h"

namespace _TRAINDATA{
	const string freqs[] = {"094","131","171","193","211","304","335","1600","1700"};
	const string secs[] = {"02","09","00","07","00","08","03","12","31"};
	const string picture_path = "C://Users//zck//Desktop//新建文件夹//毕业设计//数据//";
}

vector< map<string,string> > get_annotation_from_xml(string path);
//用于读图像矩阵,以及数据可视化
class SunPic{
	/**
	format: yyyy-mm-ddThh:mm
	*/
	string name;
	vector<Mat> src;
public:
	vector<Mat> gray;
	bool success;
	SunPic(String Name):name(Name){
		for (auto &c:name){ if ( c==':' ) c='_'; }
		read_data();
	}
	void read_data();
	int show(double left=-1000, double right=1000, double up=1000, double down=-1000);
};


class Anno{
public:
	//hpc_bbox
	double up, down, left, right;
	string event_starttime, event_peaktime, event_endtime, concept;
	double area() const{
		return fabs(up-down)*fabs(right-left);
	}
	bool set(map<string,string> key_value){
		event_starttime = key_value["event_starttime"];
		event_peaktime = key_value["event_peaktime"];
		event_endtime = key_value["event_endtime"];
		concept = key_value["concept"];
		string bbox = key_value["hpc_bbox"];
		double x[5],y[5];
		sscanf(bbox.c_str(), "POLYGON((%lf %lf,%lf %lf,%lf %lf,%lf %lf,%lf %lf))", x,y,x+1,y+1,x+2,y+2,x+3,y+3,x+4,y+4);
		left =  *min_element(x, x+5);
		right = *max_element(x, x+5);
		up  =  *max_element(y, y+5);
		down = *min_element(y, y+5);
		if ( left<-1200 || left>1200 || right<-1200 || right>1200 || up<-1200 || up>1200 || down<-1200 || down>1200 ) return false;
		return true;
	}
	void print(){
		cout<<"event_starttime: "<<event_starttime<<endl;
		cout<<"event_peaktime: "<<event_peaktime<<endl;
		cout<<"event_endtime: "<<event_endtime<<endl;
		cout<<"concept: "<<concept<<";"<<endl;
		cout<<"hpc_bbox: "<<left<<"  "<<right<<"  "<<up<<"  "<<down<<endl;
		cout<<"area: "<<area()<<endl<<endl;
	}
	void write(FILE *fout){
		fprintf(fout, "%s,%s,%s,%s,%.4f,%.4f,%.4f,%.4f\n",
			event_starttime.c_str(),event_peaktime.c_str(),event_endtime.c_str(),concept.c_str(),
			left,right,up,down);
	}
	bool read(FILE *fin){
		char buff[1024];
		if ( fscanf(fin, "%[^,]", buff)==-1 ) return false;	event_starttime = buff;	fgetc(fin);	*buff=0;
		if ( fscanf(fin, "%[^,]", buff)==-1 ) return false;	event_peaktime = buff;	fgetc(fin);	*buff=0;
		if ( fscanf(fin, "%[^,]", buff)==-1 ) return false;	event_endtime = buff;	fgetc(fin);	*buff=0;
		if ( fscanf(fin, "%[^,]", buff)==-1 ) return false;	concept = buff;			fgetc(fin);	*buff=0;
		if ( fscanf(fin, "%lf,%lf,%lf,%lf", &left, &right, &up, &down)==-1 ) return false;
		if ( concept=="Flare " ) concept = "Flare";
		fgets(buff, 1024, fin);
		return true;
	}
	bool operator == ( const Anno& t) const{
		if ( event_starttime != t.event_starttime ) return false;
		if ( event_peaktime != t.event_peaktime ) return false;
		if ( event_endtime != t.event_endtime ) return false;
		if ( concept != t.concept ) return false;
		if ( fabs( up - t.up ) > 1e-5 ) return false;
		if ( fabs( down - t.down ) > 1e-5 ) return false;
		if ( fabs( left - t.left ) > 1e-5 ) return false;
		if ( fabs( right - t.right ) > 1e-5 ) return false;
		return true;
	}	
	bool operator < ( const Anno& t) const{
		if ( event_starttime != t.event_starttime ) return event_starttime < t.event_starttime;
		if ( event_peaktime != t.event_peaktime ) return event_peaktime < t.event_peaktime;
		if ( event_endtime != t.event_endtime ) return event_endtime < t.event_endtime;
		if ( concept != t.concept ) return concept < t.concept;
		if ( fabs( up - t.up ) > 1e-5 ) return up < t.up;
		if ( fabs( down - t.down ) > 1e-5 ) return down < t.down;
		if ( fabs( left - t.left ) > 1e-5 ) return left < t.left;
		if ( fabs( right - t.right ) > 1e-5 ) return right < t.right;
		return true;
	}
};

class Traindata{
	vector<Anno> annos;
public:
	//read train information from data
	void read();
	//read annotation information
	void read_annotation();	
	//read lmssal xml information
	void read_annotation_from_lmsal_xml(string path);
	//read jpg picture
	void read_picture();

	void test(){
		sort( annos.begin(), annos.end(), [](const Anno a, const Anno b){
			if ( (a.concept.find("Flare")!=string::npos)!=(b.concept.find("Flare")!=string::npos) ){
				return (a.concept.find("Flare")!=string::npos) > (b.concept.find("Flare")!=string::npos);
			}
			return a.area()>b.area();
		});
		SunPic sun("2011-08-09T06:00");
		for ( auto anno : annos ){
			//特征尺度不变性检查
			if ( anno.event_starttime<="2011-08-09T06:00" && anno.event_endtime>="2011-08-09T06:00" ){
				for ( double scale = 1; scale>0.9; scale*=0.8 ){
					double left = anno.left, right = anno.right,
							up = anno.up, down = anno.down;
					int L=(int)(left/0.6 + sun.gray[0].cols/2), R=(int)(right/0.6 + sun.gray[0].cols/2),
						U=(int)(up/0.6 + sun.gray[0].rows/2), D=(int)(down/0.6 + sun.gray[0].rows/2);
					if (L<0 || L>=sun.gray[0].cols || R<0 || R>=sun.gray[0].cols || U<0 || U>=sun.gray[0].rows || D<0 || D>=sun.gray[0].rows) continue ;
					vector<double> feat;
					for (int i=0; i<(int)sun.gray.size(); i++){
						Mat _s = Mat(sun.gray[i], Rect(L, D, R-L, U-D) );
						Mat s;
						resize(_s, s, Size( (int)(scale*_s.cols), (int)(scale*_s.rows) ) );

						auto res = get_ColorMoment_from_mat(s);
						for_each(begin(res), end(res), [&feat](double x){feat.push_back(x);} );
						res = get_texture_from_mat(s);
						for (int i=0; i<4; i++){
							feat.push_back(res[i]);
						}

						int n = s.rows, m = s.cols;
						double sum = 0, weight = 0;
						for (int u=0; u<n; u++){
							for (int v=0; v<m; v++){
								double dis = sqrt( (double)u*u/(n*n) + (double)v*v/(m*m) );
								sum += s.at<uchar>(u,v) * exp( -dis );
								weight += exp( -dis );
							}
						}
						sum /= weight;
						cout<<" sum["<<i<<"]"<<" = "<<sum<<endl;
					}
					anno.print();
				
					for (int i=0; i<(int)feat.size(); i++){
						cout<<feat[i];
						if ( (i+1)%7==0 ) cout<<endl;else cout<<" ";
					}
					cout<<endl;
				

					int x = sun.show(anno.left, anno.right, anno.up, anno.down);
					if ( 'q' == x ) return ;
					//break;
				}
			}
		}
		
	}
	
	void save_xml_to_txt(string filename){
		FILE *f = fopen(filename.c_str(), "w");
		for (auto t : annos){
			t.write(f);
		}
		fclose(f);
	}
	void read_annotation_from_txt(string filename){		
		FILE *f = fopen(filename.c_str(), "r");
		Anno t;
		while( t.read(f) ){
			annos.push_back(t);
		}
		//for ( auto t : annos ){t.print();}
		fclose(f);
		cout<<"read annotation from txt, finished."<<endl;
	}
};
