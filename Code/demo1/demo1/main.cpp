#include "demo1.h"
#include "feature.h"
#include "nca.h"
#include "traindata.h"
#include "traindata_nasa.h"

void show_flare_example(){	
	SunPic test("2011-08-09T06:00");
	test.show(-470, 573, 476, -174);
}

int main()
{
	//wine_demo();
	//vector< map<string,string> > anno = get_annotation_from_xml("..//..//..//data//regional annotation//data//");
	//show_active_regional();
	//Traindata traindata_test;
	//traindata_test.read_annotation_from_lmsal_xml("..//..//..//data//regional annotation//data//");
	//traindata_test.save_xml_to_txt("lmsal.txt");

	//traindata_test.read_annotation_from_txt("lmsal.txt");
	//traindata_test.test();

	//show_flare_example();

	//checkallimg();

	//generate_negative_traindata(0);

	//generate_traindata("anno_nasa.good", 0);
	
	//calc_train_feature();

	
	//train(0, 256*1234+16);
	
	//match();

	return 0;

	// 读入一张图片
	Mat img = read_jpg_to_gray_mat("C://Users//zck//Documents//GitHub//SunActivity//Image Annotation//SampleData//latest_512_0094.jpg");	

	vector<double> cm = get_ColorMoment_from_mat(img);
	cout<<"("<<cm[0]<<","<<cm[1]<<","<<cm[2]<<")"<<endl;

	vector<double> texture = get_texture_from_mat(img);
	for (int i=0; i<(int)texture.size(); i++){
		cout<<texture[i];
		if ( i == (int)texture.size()-1 ) cout<<endl;
		else cout<<" ";
	}
	
	// 等待按键后窗口自动关闭
	waitKey(0);
}

