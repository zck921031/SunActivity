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
		read();
	}
	void read();
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
	void set(map<string,string> key_value){
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
	}
	void print(){
		cout<<"event_starttime: "<<event_starttime<<endl;
		cout<<"event_peaktime: "<<event_peaktime<<endl;
		cout<<"event_endtime: "<<event_endtime<<endl;
		cout<<"concept: "<<concept<<";"<<endl;
		cout<<"hpc_bbox: "<<left<<"  "<<right<<"  "<<up<<"  "<<down<<endl;
		cout<<"area: "<<area()<<endl<<endl;
	}
};

class Traindata{
	vector<Anno> annos;
public:
	//read train information from data
	void read();
	//read lmssal xml information
	void read_annotation();
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
			if ( anno.event_starttime<="2011-08-09T06:00" && anno.event_endtime>="2011-08-09T06:00" ){
				
				vector<Mat> t = vector<Mat>( sun.gray.size() );
				double left = anno.left, right = anno.right,
						up = anno.up, down = anno.down;
				int L=(int)(left/0.6 + sun.gray[0].cols/2), R=(int)(right/0.6 + sun.gray[0].cols/2),
					U=(int)(up/0.6 + sun.gray[0].rows/2), D=(int)(down/0.6 + sun.gray[0].rows/2);
				if (L<0 || L>=sun.gray[0].cols || R<0 || R>=sun.gray[0].cols || U<0 || U>=sun.gray[0].rows || D<0 || D>=sun.gray[0].rows) continue ;
				vector<double> feat;
				for (int i=0; i<(int)t.size(); i++){
					t[i] = Mat(sun.gray[i], Rect(L, D, R-L, U-D) );
					auto res = get_ColorMoment_from_mat(t[i]);
					for_each(begin(res), end(res), [&feat](double x){feat.push_back(x);} );
				}
				anno.print();
				
				for (int i=0; i<(int)feat.size(); i++){
					cout<<feat[i];
					if ( (i+1)%3==0 ) cout<<endl;else cout<<" ";
				}
				cout<<endl;
				

				int x = sun.show(anno.left, anno.right, anno.up, anno.down);
				if ( 'q' == x ) return ;
				//break;
			}
		}
		
	}
};
