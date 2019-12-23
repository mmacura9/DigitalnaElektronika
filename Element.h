#ifndef ELEMENT_H
#define ELEMENT_H

#include "Exceptions.h"
#include<vector>
using namespace std;

const int standardNumberOfIns = 2;//Standardan broj ulaza za AND i OR elemente

enum type { Sonda = 0, Gen, Manual, Not, And, Or };

class Element
{
public:
	Element(int identifier) :identifier_(identifier) {}
	Element(const Element& e);
	virtual ~Element() {}
	virtual Element* clone() const = 0;

	virtual void doing(const bool input) = 0;

	int getIdentifier() const { return identifier_; }
	bool getOutput() const { return output_; }
	int getNumberOfIns() const { return numberOfIns_; }
	virtual vector<double> getTimes() const = 0;
	virtual double getPeriod() const = 0;
	//funkcija koja odredjuje izlazni signal na osnovu ulaznog signala u element
	type getType() const { return x_; }

	virtual void setOutput() = 0;
protected:
	bool output_;//izlazni signal
	type x_;//tip elementa(Sonda,Gen,Manual,Not,And,Or)
	int identifier_,numberOfIns_;//redni broj elementa, broj ulaza u element
};

class NOT :public Element
{
public:
	NOT(int identifier) :Element(identifier) { x_ = Not; output_ = false; numberOfIns_ = 1; }
	NOT(const Element& other) :Element(other) {}
	void doing(const bool input) override { output_ = !input; }

	NOT* clone() const override { return new NOT(*this); }

	double getPeriod() const override { throw WrongInput("Greska"); return 0; }
	vector<double> getTimes() const override { throw WrongInput("Greska"); return vector<double>(); }

	void setOutput() override { output_ = output_; }
private:
};

class AND :public Element
{
public:
	//izlazni signal na pocetku stavljam na true, da bih mogao da ga AND-ujem sa svakim ulaznim signalom
	AND(const int identifier, int numberOfIns = standardNumberOfIns);
	AND(const Element& other) :Element(other) { numberOfIns_ = other.getNumberOfIns(); }
	AND* clone() const override { return new AND(*this); }

	void doing(const bool input) override { output_ = output_ && input; }//AND-ovanje sa ulaznim signalom
	
	double getPeriod() const override { throw WrongInput("Greska"); return 0; }
	vector<double> getTimes() const override { throw WrongInput("Greska"); return vector<double>(); }
	
	void setOutput() override { output_ = true; }
	void setNumerOfIns(const int numberOfIns) { numberOfIns_ = numberOfIns; }
private:
};

class OR :public Element
{
public:
	OR(const int identifier,const int numberOfIns = standardNumberOfIns);
	OR(const Element& other) :Element(other) { numberOfIns_ = (other.getNumberOfIns()); }
	OR* clone() const override { return new OR(*this); }

	void doing(const bool input) override { output_ = output_ || input; }

	double getPeriod() const override { throw WrongInput("Greska"); return 0; }
	vector<double> getTimes() const override { throw WrongInput("Greska"); return vector<double>();}

	void setOutput() override { output_ = false; }
	void setNumerOfIns(const int numberOfIns) { numberOfIns_ = numberOfIns; }
private:
};

class GEN :public Element
{
public:
	GEN(int identifier, double freq) :Element(identifier), freq_(freq) { x_ = Gen; period_ = 1 / freq_; output_ = false; numberOfIns_ = 0; }
	GEN(const Element& other) :Element(other), freq_(1 / other.getPeriod()), period_(other.getPeriod()) {}
	GEN* clone() const override { return new GEN(*this); }
	
	void doing(const bool input) override;//ako je input true onda menjam signal

	void setOutput() override { output_ = output_; }

	double getPeriod() const override { return period_; }
	vector<double> getTimes() const override{ throw WrongInput("Greska"); return vector<double>();}
private:
	double freq_, period_;//frekvencija i period
};

class MANUAL :public Element
{
public:
	MANUAL(int identifier, vector<double> times);
	MANUAL(const Element& other) :Element(other), times_(other.getTimes()),numOfTimes_(times_.size()) { }
	MANUAL* clone() const override { return new MANUAL(*this); }
	
	void doing(const bool input) override;//ako je input true onda menjam signal

	void setOutput() override { output_ = output_; }

	double getPeriod() const override { throw WrongInput("Greska"); return 0; }
	vector<double> getTimes() const override;
private:
	vector<double> realTimes() const;
	int numOfTimes_;
	vector<double> times_;// vremena za menjanje signala
};

class SONDA :public Element
{
public:
	SONDA(int identifier) :Element(identifier) { x_ = Sonda; numberOfIns_ = 1; }
	SONDA(const Element& other) :Element(other) {};
	SONDA* clone() const override { return new SONDA(*this); }

	void doing(const bool input) override { output_ = input; }//ulazni signal je jednak izalznom

	void setOutput() override { output_ = output_; }

	double getPeriod() const override { throw WrongInput("Greska"); return 0; }
	vector<double> getTimes() const override{ throw WrongInput("Greska"); return vector<double>();}
private:

};

#endif