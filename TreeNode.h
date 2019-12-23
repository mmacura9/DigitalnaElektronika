#ifndef TREE_NODE_H
#define TREE_NODE_H

#include "Element.h"

class TreeNode {
public:
	TreeNode(Element& e);
	TreeNode(const TreeNode&) = delete;
	TreeNode(TreeNode&&) = delete;
	~TreeNode();

	int getFreeChild() const { return freeChild_; }
	Element* getElement() const { return e_; }
	int getNumberChilds() const { return numberOfChilds_; }
	TreeNode* getChild(int i) const;

	void setChild(int i, TreeNode* Child) { childs_[i]=Child; }
	
	void increseFreeChild() { freeChild_++; }

private:
	int number_, numberOfChilds_, freeChild_;//broj dece= e_->numberOfIns,dete koje pokazuje na nullptr
	Element* e_;
	vector<TreeNode*> childs_;//znam da sam nepismen bilo je krace od children (bar mi se cinilo tako)
};
#endif