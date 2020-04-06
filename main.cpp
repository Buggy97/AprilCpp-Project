#include <bits/stdc++.h> 
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
	while(it1!=vecint.end() && it2!=bstint.end())
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
	for(int i = 9; i >= 0; i--)
	{
		vecint_.push_back(i);
		bstint_.insert(i);
		pvecint_->push_back(i);
		pbstint_->insert(i);
	}
	std::vector<int>::reverse_iterator it1_ = vecint_.rbegin();
	BST<int>::const_iterator it2_ = bstint_.begin();
	while(it1_!=vecint_.rend() && it2_!=bstint_.end())
	{
		if(*it1_!=*it2_)
			return false;
		it1_++;
		it2_++;
	}
	srand(time(0));
	//Ordine sparso
	std::vector<int> vecint__;
	BST<int> bstint__;
	std::vector<int>* pvecint__ = new std::vector<int>();
	BST<int>* pbstint__ = new BST<int>();
	int var[] = {5,2,8,1,3,7,9};
	for(int i = 0; i < 7; i++)
	{
		int tmp = var[i];
		vecint__.push_back(tmp);
		bstint__.insert(tmp);
		pvecint__->push_back(tmp);
		pbstint__->insert(tmp);
	}
	sort(vecint__.begin(), vecint__.end());
	std::vector<int>::const_iterator it1__ = vecint__.begin();
	BST<int>::const_iterator it2__ = bstint__.begin();
	while(it1__!=vecint__.end() && it2__!=bstint__.end())
	{
		if(*it1__!=*it2__)
			return false;
		it1__++;
		it2__++;
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
	
	return true;
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
	std::cout << "Test 1" << (test1()?"PASS":"FAIL") << std::endl;
}
