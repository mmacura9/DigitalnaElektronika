#include "Writer.h"
#include<string>

void Writer::write(const string& filepath)
{
	string filename(filepath);
	while (filename.back() != '.')
		filename.pop_back();
	filename.pop_back();
	filename = filename + "_" + to_string(sonda_) + ".txt";
	fstream output(filename, ios::out);
	for (int i = 0; i < size(changes_); i++)
	{
		output << output_;
		output << " -> ";
		output << !output_;
		output << ": ";
		output << changes_[i];
		output_ = !output_;
		output << "us" << endl;
	}
	output.close();
}
