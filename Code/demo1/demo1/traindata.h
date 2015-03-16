#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<sys/stat.h>
#include<cv.h>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;
#ifdef WIN32
	#include "dirent.h"
#else
	#include <dirent.h>
#endif
vector< map<string,string> > get_annotation_from_xml(string path);
