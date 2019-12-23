#include "Tree.h"
#include<stack>
using namespace std;

Tree::~Tree()
{
	delete root_;
	root_ = nullptr;
}

void Tree::setOutput()
{
	root_->getElement()->setOutput();
}

void Tree::doing()
{
	TreeNode* node = root_;
	stack<TreeNode*> unvisitedNodes;
	stack<int> unvisitedChilds;
	int i = 0;
	if (node == nullptr)
		throw Error("Greska");
	while (!unvisitedNodes.empty() || i<node->getNumberChilds())
	{
		if (node!=nullptr && node->getNumberChilds() && i<node->getNumberChilds())
		{
			unvisitedNodes.push(node);//ako cvor ima decu cvorove onda prelazimo na njih
			unvisitedChilds.push(i + 1);//da znamo na koji cvor idemo posle
			node = node->getChild(i);
			i = 0;
		}
		else
		{
			if (node != nullptr)
			{
				unvisitedNodes.top()->getElement()->doing(node->getElement()->getOutput());
				//sredjujemo podstablo
				node = unvisitedNodes.top();
				i = unvisitedChilds.top();
				unvisitedNodes.pop();//skidamo sa steka sredjeno podstablo
				unvisitedChilds.pop();
			}
			else
			{
				throw Error("Neki od ulaza nije povezan");
				return;//do ovoga ce doci samo ako neko od ulaza nije povezano
			}
		}
	}
}

double Tree::getPeriod() const
{
	return root_->getElement()->getPeriod();
}

vector<double> Tree::getTimes() const
{
	return root_->getElement()->getTimes();
}

int Tree::getNumberOfIns() const
{
	return root_->getElement()->getNumberOfIns();
}

int Tree::getFreeChild() const
{
	return root_->getFreeChild();
}

int Tree::getIdentifier() const
{
	return root_->getElement()->getIdentifier();
}

bool Tree::getOutput() const
{
	return root_->getElement()->getOutput();
}

type Tree::getType() const
{
	return root_->getElement()->getType();
}
