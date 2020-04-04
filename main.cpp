#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "BST.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct default_comparator
{
	inline int operator()(const int& a, const int& b)
	{
		if (a > b)
			return 1;
		else if (a < b)
			return -1;
		else
			return 0;
	}
};

class Foo
{
	private:
		Foo(){}
	public: 
		Foo(int i)
		{
			std::cout << i << std::endl;
		}
};


const BST<int, default_comparator>* curr = nullptr;

bool maggioredi(const int& p)
{
	return p>-999;
}

int scuffedcompareFunction(const int& a, const int& b)
{
	if(a > b)
		return 1;
	else if(a < b)
		return -1;
	else return 0;
}

int scuffedcompareFunction2(const Foo& a, const Foo& b)
{
	return 1;
}

bool sempreTrue(const int& p)
{
	return true;
}

void print(int p)
{
	std::cout << "Number: " << p << std::endl;
}


int main(int argc, char** argv) 
{
	BST<int, default_comparator>* bst = new BST<int, default_comparator>();
	std::vector<int> vec;// = new std::vector<int>();
	
	srand(time(NULL));
	int vals[] = {5,2,3,4,1,8,6,9,7};
	
	for(int i = 0; i < 9; i++)
	{
		//double to_add = ((double)(rand()%10000))/100.d;
		int to_add = rand()%100;
		int* ot = new int(vals[i]);
		bst->insert(*ot);
		vec.push_back(*ot);
	}
	
	BST<int, default_comparator>::const_iterator it = bst->begin();
	std::vector<int>::const_iterator vit = vec.begin();
	
	//std::cout << *bst << std::endl;
	
	while(it!=bst->end())
		std::cout << *it << " ", it++;
	std::cout << std::endl << std::endl;
	std::cout << vec.size() << std::endl;
	
	for(int p : vec)
		std::cout << p << " ";

	
	delete bst;
	//delete vec;

	//std::cout << sub << std::endl;*/

}

