#ifndef WRITER_H
#define WRITER_H

#include<fstream>
#include<vector>
using namespace std;

class Writer
{
public:
	Writer(const int sonda) { output_ = false; sonda_ = sonda; }
	//destruktor nije potreban jer nema dinamickih elemenata
	void write(const string& filepath);
	void addChange(double change) { changes_.push_back(change); }
private:
	bool output_;
	int sonda_;
	vector<double> changes_;
};

#endif