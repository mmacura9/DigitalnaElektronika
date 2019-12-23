#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Time.h"
#include "Tree.h"


class Simulator
{
public:
	Simulator();
	~Simulator();

	void loadCircuit(const string& filepath);
	void simulate(const string& filepath);
private:
	void gens();
	void sortTime();
	void solve(const string& line, const int i);
	void connect(const string& line);
	double time_;
	vector<Time*> myTime_;
	vector<Tree*> roots_;
	vector<int> sonde_;
	int numberOfElements_;
};
#endif
