#include "traindata.h"
using namespace  _TRAINDATA;

//列出path目录下所有以suffix为后缀的文件
vector<string> list_file(string path, string suffix){
	vector<string> ret;
	DIR *dp;
    struct dirent *dirp;
    if((dp=opendir(path.c_str()))==NULL){
        perror("opendir error");
        free(dp);
        return ret;
    }	
    while((dirp=readdir(dp))!=NULL){
        if((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0)) continue;
        string file_name = dirp->d_name;		
		if ( file_name.find_last_of(suffix) != file_name.size() - 1 ) continue;
		ret.push_back(path+file_name);
    }
    closedir(dp);
	return ret;
}

//从xml文件读取lmsal的标注信息，一个标注的所有<key,value>放到一个map里
void process_xml(const string&filename, vector< map<string,string> >&res ){
	string xml;
	FILE *fin = fopen(filename.c_str(), "r");
	char buf[10240];
	while( fgets(buf, 10240, fin)!=NULL ){
		xml += buf;
	}
	fclose(fin);
	int p = -1, q = -1;
	while( ( p=xml.find("<result>", p+1) )!= xml.npos ){
		int q = xml.find("</result>", p);
		string s = xml.substr(p, q-p+9);		
		map<string,string> ac;
		int a=-1,b,c;
		while( (a=s.find("<param", a+1)) != s.npos ){
			b = s.find(">", a);
			c = s.find("</param>", a);
			int x = a+13;
			int len = b-x-1;
			string key = s.substr(x, len);
			string value = s.substr(b+1, c-b-1 );
			ac[key] = value;
		}
		res.push_back(ac);
	}
}

vector< map<string,string> > get_annotation_from_xml(string path){
	vector< map<string,string> > ret;
	vector<string> filenames = list_file(path, "xml");
	for (auto filename : filenames){
		process_xml(filename, ret);
	}
	cout<<ret.size()<<endl;
	return ret;
}



void Traindata::read_annotation(){
	vector< map<string,string> > lmsal_annos = get_annotation_from_xml("..//..//..//data//regional annotation//data//");
	for (auto lmsal_anno:lmsal_annos){
		Anno t;
		t.set(lmsal_anno);
		//{t.print();return ;}
		annos.push_back(t);
	}
}
void Traindata::read_annotation_from_lmsal_xml(string path){	
	vector<string> filenames = list_file(path, "xml");
	cout<<filenames.size()<<endl;
	int iter = filenames.size();
	for (auto filename : filenames){
		if ( iter-- <= 0 ) break;
		vector< map<string,string> > res;
		process_xml(filename, res);
		for (auto lmsal_anno:res){
			Anno t;
			if ( t.set(lmsal_anno) ){
				annos.push_back(t);
			}
		}
		cout<<iter<<endl;
	}
	sort( annos.begin(), annos.end() );
	cout<<annos.size()<<endl;
	annos.erase(unique( annos.begin(), annos.end() ), annos.end() );	
	cout<<annos.size()<<endl;
	/*
	for (Anno t : annos){
		if ( t.concept.find("Flare") != String::npos ){
			t.print();
			char s[1024];
			gets(s);
		}
	}
	*/
}