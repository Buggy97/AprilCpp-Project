#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "BST.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

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
	return 0;
}
