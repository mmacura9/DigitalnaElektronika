#include "Exceptions.h"
#include "Simulator.h"
#include "Writer.h"
#include <fstream>

Simulator::Simulator():time_(0),myTime_(),roots_(),sonde_(),numberOfElements_(0){}

Simulator::~Simulator()
{
	for (int i = 0; i < size(roots_); i++)
		delete roots_[i];
	for (int i = 0; i < size(myTime_); i++)
		delete myTime_[i];
}

void Simulator::loadCircuit(const string& filepath)
{
	for (int i = 0; i < size(roots_); i++)
		delete roots_[i];
	for (int i = 0; i < size(myTime_); i++)
		delete myTime_[i];
	roots_.clear();
	myTime_.clear();
	sonde_.clear();
	time_ = 0;
	numberOfElements_ = 0;
	//ako nesto ima u simulatoru da se obrise
	fstream input(filepath, ios::in);
	if (input.peek() == EOF)
		throw WrongInput("Lose uneseni podaci!");
	input >> time_;//unosenje vremena
	if (time_ <= 0)
		throw WrongInput("Vreme mora da bude pozitivan broj!");
	if (input.peek() == EOF)
		throw WrongInput("Lose uneseni podaci!");
	input >> numberOfElements_;//unosenje broja elemenata
	if (numberOfElements_ <= 0)
		throw WrongInput("Broj elemenata mora da bude pozitivan broj!");
	string line;
	getline(input, line);//zavrsavanje linije
	for (int i = 0; i < numberOfElements_; i++)//unosenje elemenata
	{
		if (input.peek() == EOF)
			throw WrongInput("Lose uneseni podaci!");
		getline(input, line);
		this->solve(line, i);//funkcija solve obradjuje liniju
		if (this->roots_[i]->getType() == Sonda)
			sonde_.push_back(i);
	}
	while (input.peek() != EOF)
	{
		string line;
		getline(input, line);
		connect(line);//funkcija connect obradjuje liniju i spaja elemente
	}
}

void Simulator::simulate(const string& filepath)
{
	gens();//funkcija gens nalazi sve momente u kojima se menjaju generatori
	sortTime();//funkcija sortTime sortira vektor myTime_ po trenutcima
	vector<Writer*> writer_;
	if (myTime_[0]->getTime() != 0)
	{
		for (int i = 0; i < size(sonde_); i++)
		{
			for (int p = 0; p < size(roots_); p++)
			{
				if (roots_[p]->getType() == Or || roots_[p]->getType() == And)
					roots_[p]->setOutput();
			}
			roots_[sonde_[i]]->doing(); //odredjujemo pocetne vrednosti sondi
			writer_.push_back(new Writer(roots_[sonde_[i]]->getIdentifier()));
			if (roots_[sonde_[i]]->getOutput() == true)
				writer_[i]->addChange(0);//posto je pocetno stanje sonde false, ako se u trenutku 0 menja sonda dodajem i taj momenat
		}
	}

	for (int i = 0; i < size(myTime_); i++)
	{
		myTime_[i]->change();//menja vrednost generatora koji se menjaju u tom momentu
		for (int j = 0; j < size(sonde_); j++)
		{
			bool prev = roots_[sonde_[j]]->getOutput();
			for (int p = 0; p < size(roots_); p++)
			{
				if (roots_[p]->getType() == Or || roots_[p]->getType() == And)
					roots_[p]->setOutput();
			}
			roots_[sonde_[j]]->doing();//obradjuje stablo 
			if (prev != roots_[sonde_[j]]->getOutput())
				writer_[j]->addChange(myTime_[i]->getTime());//ako se promenio output ispisi
		}
	}
	for (int i = 0; i < size(sonde_); i++)
		writer_[i]->write(filepath);
	for (int i = 0; i < size(sonde_); i++)
		delete writer_[i];
}

void Simulator::gens()
{
	for (int i = 0; i < numberOfElements_; i++)
	{
		if (roots_[i]->getType() == Gen)
		{
			double t = roots_[i]->getPeriod() / 2;
			for (double p = t; p < time_; p += t)
			{
				bool ok = true;
				int k;
				for (k = 0; k < size(myTime_); k++)
					if (myTime_[k]->getTime() == p)
					{
						ok = false;
						myTime_[k]->addGen(roots_[i]->getRoot());
						break;
					}
				if (ok)
				{
					myTime_.push_back(new Time(p));
					myTime_[k]->addGen(roots_[i]->getRoot());
				}
			}
		}
		if (roots_[i]->getType() == Manual)
		{
			vector<double> t = roots_[i]->getTimes();
			for (int p = 0; p < size(t); p++)
			{
				bool ok = true;
				int j;
				for (j = 0; j < myTime_.size(); j++)
					if (myTime_[j]->getTime() == t[p])
					{
						ok = false;
						myTime_[j]->addGen(roots_[i]->getRoot());
						break;
					}
				if (ok)
				{
					myTime_.push_back(new Time(t[p]));
					myTime_[j]->addGen(roots_[i]->getRoot());
				}
			}
		}
	}
}

void Simulator::sortTime()
{
	for (int i = 0; i < size(myTime_); i++)
		for (int j = i; j < size(myTime_); j++)
			if (myTime_[i]->getTime() > myTime_[j]->getTime())
			{
				Time* swap = myTime_[i];
				myTime_[i] = myTime_[j];
				myTime_[j] = swap;
			}
}

void Simulator::solve(const string& line, const int i)
{
	int identifier = 0;
	int j = 0;
	while (line[j] != ' ')
	{
		identifier = 10 * identifier + line[j] - '0';
		j++;
	}
	while (line[j] == ' ')
		j++;
	int el = 0;
	while (line[j] != ' ' && j < line.length())
	{
		el = el * 10 + line[j] - '0';
		j++;
	}
	vector<double> times;
	Element* e;
	int k;
	double x;
	double freq;
	Tree* pok;
	bool ok = true;
	double time;
	switch (el)
	{
	case 0: 
		e = new SONDA(identifier);
		pok = new Tree(*e, i);
		roots_.push_back(pok);
		delete e;
		break;
	case 3: e = new NOT(identifier); 
		pok = new Tree(*e, i);
		roots_.push_back(pok);
		delete e;
		break;
	case 1:
		while (line[j] == ' ' && j < line.length())
			j++;
		freq = 0;
		while (line[j] != '.' && j < line.length() && line[j] != ' ')
		{
			freq = 10 * freq + line[j] - '0';
			j++;
		}
		if (j >= line.length())
		{
			e = new GEN(identifier, freq);
			pok = new Tree(*e, i);
			roots_.push_back(pok);
			delete e;
			break;
		}
		j++;
		x = 10;
		while (line[j] != ' ' && j < line.length())
		{
			freq = freq + (line[j] - '0') / x;
			x *= 10;
			j++;
		}
		e = new GEN(identifier, freq);
		pok = new Tree(*e, i);
		roots_.push_back(pok);
		delete e;
		break;
	case 2:
		//k = 0;
		time = 0;
		while (j < line.length())
		{
			while (line[j] == ' ')
				j++;
			x = 10;
			while (line[j] != ' ' && j < line.length())
			{
				if (line[j] == '.')
				{
					ok = false;
					j++;
					continue;
				}
				if (ok)
					time = 10 * time + line[j] - '0';
				if (!ok)
				{
					time += (line[j] - '0') / x;
					x *= 10;
				}
				j++;
			}
			times.push_back(time);
			time = 0;
		}
		e = new MANUAL(identifier, times);
		pok = new Tree(*e, i);
		roots_.push_back(pok);
		delete e;
		break;
	case 4:
		if (j >= line.length())
		{
			e = new AND(identifier);
			pok = new Tree(*e, i);
			roots_.push_back(pok);
			delete e;
			break;
		}
		else
		{
			while (line[j] == ' ' && j < line.length())
				j++;
			x = 0;
			while (line[j] != ' ' && j < line.length())
			{
				x = 10 * x + line[j] - '0';
				j++;
			}
			if (j < line.length())
			{
				while (line[j] != ' ' && j < line.length())
					j++;
				if (j < line.length())
					throw WrongInput("Pogresan broj argumenata za AND element");
			}
			e = new AND(identifier, x);
			pok = new Tree(*e, i);
			roots_.push_back(pok);
			delete e;
			break;
		}
	case 5:
		if (j >= line.length())
		{
			e = new OR(identifier);
			pok = new Tree(*e, i);
			roots_.push_back(pok);
			delete e;
			break;
		}
		else
		{
			x = 0;
			while (line[j] == ' ' && j < line.length())
				j++;
			while (line[j] != ' ' && j < line.length())
			{
				x = 10 * x + line[j] - '0';
				j++;
			}
			if (j < line.length())
			{
				while (line[j] != ' ' && j < line.length())
					j++;
				if (j < line.length())
					throw WrongInput("Pogresan broj argumenata za AND element");
			}
			e = new OR(identifier, x);
			pok = new Tree(*e, i);
			roots_.push_back(pok);
			delete e;
			break;
		}
	}
	//default:
		//throw WrongInput("Ne postoji ovaj element"); break;
}

void Simulator::connect(const string& line)
{
	int first = 0, second = 0, j = 0;
	while (line[j] != ' ')
	{
		first = 10 * first + line[j] - '0';
		j++;
	}
	while (line[j] == ' ')
		j++;
	while (line[j] != ' ' && j<line.length())
	{
		second = 10 * second + line[j] - '0';
		j++;
	}
	int x = -1, y = -1;
	for (int i = 0; i < numberOfElements_; i++)
	{
		if (first == roots_[i]->getIdentifier())
			x = i;
		if (second == roots_[i]->getIdentifier())
			y = i;
		if (x != -1 && y != -1)
			break;
	}
	if (x == -1 || y == -1 || x==y)
		throw WrongInput("Lose ste uneli veze");
	switch (roots_[x]->getType())
	{
	case Sonda: 
		throw WrongInput("Lose ste uneli veze");
	}
	int t;
	switch (roots_[y]->getType())
	{
	case Sonda:case Not: 
		if (j >= line.length())
		{
			throw WrongInput("Lose ste uneli veze");
		}
		while (line[j] == ' ')
			j++;
		t = 0;
		while (line[j] != ' ' && j<line.length())
		{
			t = 10 * t + line[j] - '0';
			j++;
		}
		if (t != 0)
			throw WrongInput("Lose ste uneli veze");
		if (roots_[y]->getNumberOfIns() > roots_[y]->getFreeChild())
		{
			roots_[y]->getRoot()->setChild(roots_[y]->getFreeChild() , roots_[x]->getRoot());
			roots_[y]->getRoot()->increseFreeChild();
		}
		break;
	case Gen: case Manual:
		throw WrongInput("Lose ste uneli veze"); break;
	case And:case Or:
		if (j >= line.length())
			throw WrongInput("Lose ste uneli veze");
		while (line[j] == ' ' && j<line.length())
			j++;
		if (j >= line.length())
			throw WrongInput("Lose ste uneli veze");
		int enter = 0;
		while (line[j] != ' ' && j < line.length())
		{
			enter = 10 * enter + line[j] - '0';
			j++;
		}
		if (enter >= roots_[y]->getRoot()->getNumberChilds())
			throw WrongInput("Lose ste uneli veze");
		if (roots_[y]->getRoot()->getChild(enter) != nullptr)
			throw WrongInput("Lose ste uneli veze");
		roots_[y]->getRoot()->setChild(enter, roots_[x]->getRoot());
		break;
	}
}

