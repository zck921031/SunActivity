
class Lmsal{
public:
	class Anno{
	public:
		//hpc_bbox
		double left, right, up, down;
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
	vector<Anno> annos;
	vector<Rect> res;
	void read_annotation_from_txt(string filename){		
		FILE *f = fopen(filename.c_str(), "r");
		Anno t;
		while( t.read(f) ){
			annos.push_back(t);
		}
		fclose(f);
		cout<<"read annotation from txt, finished. size: "<<annos.size()<<endl;
	}
	void find(string tim0, string concept){
		res.clear();
		for ( Anno t : annos ){
			if ( t.concept != concept ) continue;
			int left=t.left, up=t.up, right=t.right, down=t.down;
			LmsalAxis2CvAxis(left, up, right, down);
			string tim1 = t.event_starttime.substr(0,4) + t.event_starttime.substr(5,2) + t.event_starttime.substr(8,2);
			string tim2 = t.event_endtime.substr(0,4) + t.event_endtime.substr(5,2) + t.event_endtime.substr(8,2);
			if ( tim1 <= tim0.substr(0,8) && tim0.substr(0,8) <= tim2 ){
				res.push_back( Rect(left, up, right-left, down-up) );
				//t.print();
			}
		}
	}
};