#include "TreeNode.h"

TreeNode::TreeNode(Element& e)
{
	e_ = e.clone();
	numberOfChilds_ = e.getNumberOfIns();
	freeChild_ = 0;
	for (int i = 0; i < numberOfChilds_; i++)
		childs_.push_back(nullptr);
}

TreeNode::~TreeNode()
{
	if (e_ != nullptr)
		delete e_;
	e_ = nullptr;
}

TreeNode* TreeNode::getChild(int i) const
{
	if(i>=numberOfChilds_)
		return nullptr;
	return childs_[i];
}
