#include <iostream>
#include "layers/Network.hpp"
#include "Parser.hpp"
#include "Teacher.hpp"
#include "Converter.hpp"

int main() {
	Parser parser;
	cout << "Loading architecture...\n";
	Block crossConf = Block("data/cross.conf");
	Network *network = parser.blockToNetwork(crossConf);
	cout << "\tDone.\n";
	
	cout << "Loading task...\n";
	Converter converter;
	vector<string> names;
	names.push_back("cross");
	names.push_back("nothing");
	vector<Data> in, out;
	converter.loadTask("data/", names, in, out);
	cout << "\tDone.\n";
	
	cout << "Teaching network...\n";
	Teacher teacher;
	teacher.writeWeights(*network, "data/crossInit.dat");
	teacher.teach(*network, in, out, 1e3, "data/errors.txt");
	cout << "\tDone.\n";
	
	cout << "Writing results...\n";
	teacher.writeWeights(*network, "data/crossTeached.dat");
	cout << "\tDone.\n";
	return 0;
}