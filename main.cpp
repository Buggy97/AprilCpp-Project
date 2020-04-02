#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "BST.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

const BST<int>* curr = nullptr;

bool maggioredi(const int& p)
{
	return p>-999;
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
	BST<int>* bst = new BST<int>();
	srand(time(NULL));
	int vals[] = {5,2,3,4,1,8,6,9,7};
	
	for(int i = 0; i < 50; i++)
	{
		//double to_add = ((double)(rand()%10000))/100.d;
		int to_add = rand()%100;
		bst->insert(new int(to_add));
	}
	
	BST<int>::const_iterator it = bst->begin();
	
	std::cout << *bst << std::endl;
	
	while(it!=bst->end())
		std::cout << *it << " ", it++;
	
	delete bst;
	
	//delete bst;

	//std::cout << sub << std::endl;*/

}

