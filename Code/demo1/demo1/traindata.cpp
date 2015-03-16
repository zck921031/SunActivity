#include "traindata.h"
vector<string> list_file(string path){
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
		if ( file_name.find_last_of(".xml") != file_name.size() - 1 ) continue;
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
	vector<string> filenames = list_file(path);
	for (auto filename : filenames){
		process_xml(filename, ret);
	}
	cout<<ret.size()<<endl;
	return ret;
}