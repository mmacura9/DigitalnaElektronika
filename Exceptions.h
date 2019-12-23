#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include<exception>
using namespace std;

class WrongInput : public exception
{
public:
	WrongInput(const char* msg) :exception(msg) {}
};

class Error : public exception
{
public:
	Error(const char* msg) :exception(msg) {}
};

#endif