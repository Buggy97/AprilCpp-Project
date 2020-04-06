#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "BST.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

bool test1()
{
	//Ordine crescente
	std::vector<int> vecint;
	BST<int> bstint;
	std::vector<int>* pvecint = new std::vector<int>();
	BST<int>* pbstint = new BST<int>();
	for(int i = 0; i < 10; i++)
	{
		vecint.push_back(i);
		bstint.insert(i);
		pvecint->push_back(i);
		pbstint->insert(i);
	}
	std::vector<int>::const_iterator it1 = vecint.begin();
	BST<int>::const_iterator it2 = bstint.begin();
	while(it1!=vecint.end()&&it2!=vecint.end())
	{
		if(*it1!=*it2)
			return false;
		it1++;
		it2++;
	}
	//Ordine decrescente
	std::vector<int> vecint_;
	BST<int> bstint_;
	std::vector<int>* pvecint_ = new std::vector<int>();
	BST<int>* pbstint_ = new BST<int>();
	for(int i = 10; i > 0; i--)
	{
		vecint_.push_back(i);
		bstint_.insert(i);
		pvecint_->push_back(i);
		pbstint_->insert(i);
	}
	std::vector<int>::const_iterator it1_ = vecint_.begin();
	BST<int>::const_iterator it2_ = bstint_.begin();
	while(it1_!=vecint_.end()&&it2_!=vecint_.end())
	{
		if(*it1_!=*it2_)
			return false;
		it1_++;
		it2_++;
	}
	
	//Ordine sparso
	std::vector<int> vecint_;
	BST<int> bstint_;
	std::vector<int>* pvecint_ = new std::vector<int>();
	BST<int>* pbstint_ = new BST<int>();
	for(int i = 10; i > 0; i--)
	{
		vecint_.push_back(i);
		bstint_.insert(i);
		pvecint_->push_back(i);
		pbstint_->insert(i);
	}
	std::vector<int>::const_iterator it1_ = vecint_.begin();
	BST<int>::const_iterator it2_ = bstint_.begin();
	while(it1_!=vecint_.end()&&it2_!=vecint_.end())
	{
		if(*it1_!=*it2_)
			return false;
		it1_++;
		it2_++;
	}
	
	
	vecint.push_back(999);
	bstint.insert(999);
	pvecint->push_back(999);
	pbstint->insert(999);
	
	if(vecint.size()!=bstint.size())
		return false;
	if(pvecint->size()!=pbstint->size())
		return false;
		
	delete pvecint;
	delete pbstint;
	delete pvecint_;
	delete pbstint_;
	delete pvecint__;
	delete pbstint__;
}

bool test2()
{
	
}

bool test3()
{
	
}

bool test4()
{
	
}

int main(int argc, char** argv) 
{
}
