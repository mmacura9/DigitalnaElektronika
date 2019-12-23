#include "Element.h"

Element::Element(const Element& e)
{
	this->identifier_ = e.getIdentifier();
	this->numberOfIns_ = e.getNumberOfIns();
	this->output_ = e.output_;
	this->x_ = e.x_;
}

MANUAL::MANUAL(int identifier, vector<double> times) :Element(identifier), times_(times) 
{ 
	x_ = Manual;
	numberOfIns_ = 0;
	numOfTimes_ = size(times);
}

void MANUAL::doing(const bool input)
{
	if (input)
		output_ = !output_;
}

vector<double> MANUAL::getTimes() const
{
	return this->realTimes();
}

vector<double> MANUAL::realTimes() const
{
	vector<double> x;
	x.push_back(times_[0]);
	for (int i = 1; i < numOfTimes_; i++)
		x.push_back(x.back() + times_[i]);
	return x;
}

void GEN::doing(const bool input)
{
	if (input)
		output_ = !output_;
}

AND::AND(const int identifier, int numberOfIns) :Element(identifier)
{
	numberOfIns_ = (numberOfIns);
	x_ = And;
}

OR::OR(int identifier, int numberOfIns) : Element(identifier)
{
	numberOfIns_ = (numberOfIns);
	x_ = Or;
}
