#ifndef TIME_H
#define TIME_H

#include "TreeNode.h"

class Time
{
public:
	Time(double time);
	//~Time(); 
	//u ovom slucaju destruktor nije potreban jer sve dinamicke elemente vec brisem u destruktoru ~Tree()
	void change();
	void addGen(TreeNode* gen);

	double getTime() { return time_; }
	
private:
	double time_;
	vector<TreeNode*> gens_;
	int numOfGens_;
};

#endif
