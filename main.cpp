#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "BST.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

void call(void (*func)(int), int);

int new_count = 0;
int delete_count = 0;

bool maggioredi(const int& p)
{
	return p>-999;
}

void print(int p)
{
	std::cout << "Number: " << p << std::endl;
}


int main(int argc, char** argv) {
	BST<int>* bst = new BST<int>();
	srand(time(NULL));
	int vals[] = {5,2,3,4,1,8,6,9,7};
	for(int i = 0; i < 9; i++)
	{
		//double to_add = ((double)(rand()%10000))/100.d;
		int to_add = rand()%100;
		bst->insert(new int(vals[i]));
	}
	std::cout << *bst << std::endl;
	bst->elems();
	int val = 0;
	do
	{
		std::cout << "Enter value to check: ";
		std::cin >> val;
		if(val==-1)
			break;
		std::cout << (bst->exists(val)?"Present!\n":"Not present!\n");
	} while(true);
	
	do
	{
		std::cout << "Enter value to subtree: ";
		std::cin >> val;
		if(val==-1)
			break;
		BST<int>* sub = (bst->subtree(val));
		if (sub)
			std::cout << *sub << std::endl;
		else
			std::cout << "Element not found!" << std::endl;

	} while(true);
	
	bst->printif(&maggioredi);
	
	long* l = new long(27);
	long* l2 = new long(*l);

	std::cout << "*************************************************************'" << std::endl;
	std::cout << "BST: " << *bst << std::endl;
	BST<int>* bst2 = new BST<int>(*bst);
	#pragma optimize( "", off )
	new BST<int>(*bst);
	new BST<int>(*bst);
	new BST<int>(*bst);
	#pragma optimize( "", on )
	//std::cout << "BST2: " << *bst2 << std::endl;
	std::cout << "DELETE BST" << std::endl;
	delete bst;
	bst = nullptr;
	std::cout << "BST2: " << *bst2 << std::endl;
	call(&print, 42);
	delete bst2;
	bst2 = nullptr;
	delete l;
	delete l2;
	std::cout << "NEW COUNT: " << new_count << "  DELETE COUNT: " << delete_count << std::endl;
	return 0;
}

void call(void (*func)(int), int param)
{
   	func(param);
}

