#include "Time.h"

Time::Time(double time) : time_(time), numOfGens_() {}

//Time::~Time()
//{
	//for (int i = 0; i < size(gens_); i++)
		//delete gens_[i];
//}


void Time::change()
{
	for (int i = 0; i < numOfGens_; i++)
		gens_[i]->getElement()->doing(true);
}

void Time::addGen(TreeNode* gen)
{
	gens_.push_back(gen);
	numOfGens_++;
}
