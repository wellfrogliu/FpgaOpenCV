#include<iostream>
#include<time.h>

#include"neuro.hpp"
#include"descriptor.hpp"
#include"teacher.hpp"

using namespace std;
using namespace cv;

int main() {
	srand(time(0));
	
	int lsz[] = {1024, 48, 26, 1};
	int lN = sizeof(lsz) / sizeof(int);
	Neuro neuro( lN, lsz );
	
	Descriptor des( neuro );
	des.load( "data/after" );
	
	imwrite( "data/tst0.png", des.getImage() );
	des.save( "data/before" );
	cout << "data saved\n";
	
	Teacher teacher(neuro);
	
	vector< vector<vec> > inputV;
	vector<vec> theory;
	vector<string> names;
//	names.push_back("human");
//	names.push_back("dog");
//	names.push_back("tree");
//	names.push_back("bird");
	names.push_back("cross");
	names.push_back("nothing");
	
	inputV = loadClassificationTask( "data/animals/", names );
	theory = generateTheory( names.size() );
	
	int N = 1e6; //1e5 - 3min //1e7 - 323min
	int saveFreq = 5e5;
	for( int i = 0; i < N; ++i ) {
		int n = rand() % inputV.size();
		if(rand() % 2 == 0)
			n = 1;
		int k = rand() % inputV[n].size();
		teacher.teach(inputV[n][k], theory[n]);
		
		if(i % saveFreq == 0) {
			string name = string("data/temporary/temp") + to_string(i / saveFreq);
			des.save( name.c_str() );
			cout << "saved temp file " << i / saveFreq << endl;
		}
	}
	
	flt output[4];
	cout.precision(2);
	for( int i = 0; i < 30; ++i ) {
		int n = rand() % inputV.size();
		int k = rand() % inputV[n].size();
		
		neuro.predict(&inputV[n][k][0], output);
		cout << names[n] << '\t' << theory[n][0] << " : " << theory[n][1]
				<< " : " << theory[n][2]<< " : " << theory[n][3]
				<< '\t' << output[0] << '\t' << output[1] << '\t' << output[2] << '\t' << output[3] << '\n';
	}
	
	des.save( "data/after" );
	cout << "data saved\n";
	
	imwrite( "data/tst1.png", des.getImage());
	for(int i = 0; i < theory.size(); ++i) {
		Mat img = des.getLayerImage(0, theory[i], inputV[0][0] );
		resize( img, img, Size(512, 512) );
		imwrite( (string("data/") + names[i] + string(".png")).c_str(), img );
	}
	cout << "pic writed\n";
	
	Mat example = imread( "data/table-pic.png", CV_LOAD_IMAGE_GRAYSCALE );
	example = des.preprocessImage(example, 10);
	imwrite( "data/exampleout.png", des.processImage(example, 32, theory, names, 3, .1) );
	
	//des.processVideo("data/example2.avi", "data/example2out.avi", theory, names);
	
	return 0;
}




