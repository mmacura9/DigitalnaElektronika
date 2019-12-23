#ifndef TREE_H
#define TREE_H

#include "TreeNode.h"
#include<string>
using namespace std;

class Tree {
public:
	Tree() : root_(nullptr) {}
	Tree(Element& e, int number) { root_ = new TreeNode(e); }
	Tree(const Tree&) = delete;
	Tree(Tree&&) = delete;
	~Tree();

	void setOutput();

	void doing();

	double getPeriod() const;
	vector<double> getTimes() const;
	int getNumberOfIns() const;
	int getFreeChild() const;
	int getIdentifier() const;
	bool getOutput() const;
	TreeNode* getRoot() const { return root_; }
	type getType() const;

private:
	// Stablo je opisano pokazivacem na koreni cvor
	TreeNode* root_;
};

#endif