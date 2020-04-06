#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>
#include "BST.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

struct default_comparator
{
	inline int operator()(const int& a, const int& b) const
	{
		if (a > b)
			return  1;
		else if (a < b)
			return -1;
		else
			return 0;
	}
};

struct EVEN
{
	inline bool operator()(const int& a) const
	{
		if(a%2!=0)
			return true;
		return false;
	}
};

template <typename T, typename Comparator, typename Predicate> 
void printIF(BST<T, Comparator>* tree)
{
	Predicate predicate;
	if(!tree)
		return;
	printIF<T, Comparator, Predicate>(tree->left);
	if(tree->data && predicate(tree->data))
		std::cout << tree->data << " ";
	printIF<T, Comparator, Predicate>(tree->right);
}


int main(int argc, char** argv) 
{
	BST<int, default_comparator>* bst = new BST<int, default_comparator>();
	
	srand(time(NULL));
	int vals[] = {5,2,3,4,1,8,6,9,7};
	
	for(int i = 0; i < 9; i++)
	{
		int to_add = rand()%100;
		int* ot = new int(vals[i]);
		bst->insert(*ot);
	}
	
	bst->print();
	
	printIF<int, default_comparator, EVEN>(bst);
	
	int g = 8;
	BST<int, default_comparator>* sub8 = (bst->subtree(g));
	
	delete bst;
	
	std::cout << std::endl << "SUB 8: " <<*sub8 << std::endl;
	
	sub8->print();
	
	system("pause");
}
