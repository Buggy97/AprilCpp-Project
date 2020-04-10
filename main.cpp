#include <algorithm> //per ordinare i vector
#include <iostream> //per cout
#include <math.h> //RAND
#include <time.h> //Per inizializzare la rand
#include <vector> //contenitore vector per confrontare i valori nei test
#include "BST.h"

/**
	@brief Test per controllare la correttezza dell'inserimento
**/
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
	std::sort(vecint__.begin(), vecint__.end());
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
	if(vecint_.size()!=bstint_.size())
		return false;
	if(pvecint_->size()!=pbstint_->size())
		return false;
	if(vecint__.size()!=bstint__.size())
		return false;
	if(pvecint__->size()!=pbstint__->size())
		return false;
	
	delete pvecint;
	delete pbstint;
	delete pvecint_;
	delete pbstint_;
	delete pvecint__;
	delete pbstint__;
	
	return true;
	
	//TODO Test con oggetti
}

/**
	@brief Test per provare la correttezza exists()
**/
bool test2()
{
	//Test con int
	int errVals[] = {11,21,34,5343,-1,10};
	std::vector<int>* pvecint = new std::vector<int>();
	BST<int>* pbstint = new BST<int>();
	//Testo con BST vuoto
	for(int a : errVals)
		if(pbstint->exists(a))
			return false;
	//Testo inserendo un valore numerico
	if(pbstint->exists(24))
		return false;
	for(int i = 0; i < 10; i++)
	{
		pvecint->push_back(i);
		pbstint->insert(i);
	}
	std::vector<int>::const_iterator it1 = pvecint->begin();
	while(it1!=pvecint->end())
	{
		if(!pbstint->exists(*it1))
			return false;
		it1++;
	}
	for(int a : errVals)
		if(pbstint->exists(a))
			return false;
	delete pvecint;
	delete pbstint;
	return true;
	//TODO Test con oggetti
}

/**
	@brief Test per la correttezza dell'overloading dell'operatore << e della stampa con printIF.
**/
bool test3()
{
	struct simple_comp
	{
		inline int operator()(const int& a, const int& b) const
		{
			if(a > b)
				return 1;
			else if(a < b)
				return -1;
			return 0;
		}
	};
	
	struct pari
	{
		inline bool operator()(const int& a) const
		{
			if(a%2==0)
				return true;
			return false;
		}
	};
	
	
	struct maggioredi5
	{
		inline bool operator()(const int& a) const
		{
			return a > 5;
		}
	};
	
	BST<int, simple_comp>* pbstint = new BST<int, simple_comp>();
	for(int i = 0; i < 10; i++)
		pbstint->insert(i);
		
	std::cout << "Stampa contenuto con operator <<: " << *pbstint;
	std::cout << "Stampa contenuto con print: ";
	pbstint->print();
	std::cout << "Lista dei numeri pari : ";
	printIF< BST<int, simple_comp>, pari>(*pbstint);
	std::cout << std::endl;
	
	delete pbstint;
	
	return true;
}

/**
	@brief Test per la correttezza del metodo subtree
**/
bool test4()
{
	//Ordine crescente
	std::vector<int>* pvecint = new std::vector<int>();
	BST<int>* pbstint = new BST<int>();
	for(int i = 0; i < 10; i++)
	{
		pvecint->push_back(i);
		pbstint->insert(i);
	}
	BST<int>* sub5 = pbstint->subtree(5);
	if(sub5->size()!=5)
		return false;
	for (int i = 5; i < 10; i++)
		if(!sub5->exists(i))
			return false;
	//Ordine decrescente
	std::vector<int> vecint_;
	BST<int> bstint_;
	for(int i = 9; i >= 0; i--)
	{
		vecint_.push_back(i);
		bstint_.insert(i);
	}
	BST<int>* sub5_ = bstint_.subtree(5);
	if(sub5_->size()!=6)
		return false;
	for (int i = 0; i < 6; i++)
		if(!sub5_->exists(i))
			return false;
	//Ordine sparso
	std::vector<int>* pvecint__ = new std::vector<int>();
	BST<int>* pbstint__ = new BST<int>();
	int var[] = {5,2,8,1,3,7,9};
	for(int i = 0; i < 7; i++)
	{
		int tmp = var[i];
		pvecint__->push_back(tmp);
		pbstint__->insert(tmp);
	}
	BST<int>* sub5__ = pbstint__->subtree(5);
	BST<int>* sub5___ = pbstint__->subtree(-99);
	if(sub5___)
		return false;
	if(sub5__->size()!=7)
		return false;
	int farr[] = {1,2,3,5,7,8,9};
	for (int i = 0; i < 7; i++)
		if(!sub5__->exists(farr[i]))
			return false;
	BST<int>* sub3__ = pbstint__->subtree(2);
	delete pbstint__;
	if(sub3__->size()!=3)
		return false;
	for (int i = 1; i < 4; i++)
		if(!sub3__->exists(i))
			return false;
			
	delete pvecint;
	delete pbstint;
	delete sub5;
	delete sub5_;
	delete pvecint__;
	delete sub5__;
	delete sub3__;
	return true;
}

/**
	@brief Classe di prova per test5
**/
struct Integer
{
	int random_val;
	Integer(const int& val_)
	{
		random_val = val_;
	}
	Integer()
	{
		random_val = rand()%100;
	}
	Integer(const Integer& other)
	{
		random_val = other.random_val;
	}
	bool operator< (const Integer& other) const
	{
		return random_val < other.random_val;
	}
	bool operator> (const Integer& other) const
	{
		return random_val > other.random_val;
	}
	bool operator== (const Integer& other) const
	{
		return random_val == other.random_val;
	}
};

/**
	@brief Overloading per stampare la classe di prova
**/
std::ostream& operator<< (std::ostream& os, const Integer& other)
{
	os << other.random_val;
	return os;
}

/**
	@brief Test per controllo generale
**/	
bool test5()
{
	srand(time(0));
	struct IntegerDescendComp
	{
		inline int operator()(const Integer& a, const Integer& b) const
		{
			if(a>b)
				return -1;
			else if(a<b)
				return 1;
			else
				return 0;
		}
	};

	struct IntegerDescendCompPointer
	{
		inline int operator()(const Integer* const a, const Integer* const b) const
		{
			if(*a>*b)
				return -1;
			else if(*a<*b)
				return 1;
			else
				return 0;
		}
	};
	
	BST<Integer*, IntegerDescendCompPointer> integerBST;
	
	if(integerBST.size()!=0)
		return false;
		
	for(int i = 0; i < 10; i++)
		integerBST.insert(new Integer(i));
		
		
	BST<Integer*, IntegerDescendCompPointer>::const_iterator itP = integerBST.begin();
	
	while(itP!=integerBST.end())
		std::cout << **itP << " ", delete *itP, itP++;
	
	BST<Integer> intBST_;
	
	int var[] = {5,2,8,1,3,7,9};
	for(int i = 0; i < 7; i++)
		intBST_.insert(Integer(var[i]));
	
	for(int i = 0; i < 7; i++)
		if(!intBST_.exists(Integer(var[i])))
			return false;
	if(intBST_.exists(Integer(98)))
		return false;

	BST<Integer> newbst = intBST_;
	;
	
	if(newbst.size()!=intBST_.size())
		return false;
	
	newbst = intBST_; //Test leak
	newbst = intBST_;
	newbst = intBST_;
	newbst = intBST_;
	newbst = newbst;
	newbst = newbst;
	newbst = newbst;


	if(newbst.size()!=intBST_.size())
		return false;	
	intBST_.insert(Integer(42));
	
	BST<Integer>::const_iterator it_ = intBST_.begin();
	BST<Integer>::const_iterator itne = newbst.begin();
	
	BST<Integer>::const_iterator g = it_;
	it_++;
	std::cout << std::endl;
	
	it_ = intBST_.begin(); //Test leak
	it_ = intBST_.begin();
	it_ = intBST_.begin();
	
	BST<Integer>* intBST_subtree8 = intBST_.subtree(Integer(8));
	BST<Integer>::const_iterator it_sub = intBST_subtree8->begin();
	std::cout << "Stampa valori intBST_subtree8 : " << std::endl;
	std::cout << *intBST_subtree8;
	std::cout << "Stampa elementi intBST_subtree8 : " << intBST_subtree8->size() << std::endl;
	struct pari_Integer
	{
		inline bool operator()(const Integer& a)
		{
			return (a.random_val%2==0);
		}
	};
	std::cout << "Stampa valori pari intBST_ : " << std::endl;
	printIF<BST<Integer>, pari_Integer>(intBST_);
	std::cout << std::endl;
	std::cout << "Stampa valori intBST_ : " << std::endl;
	while(it_!=intBST_.end())
		std::cout << it_->random_val << " ", it_++;
	std::cout << std::endl;
		
	if(!intBST_.exists(42))
		return false;
	
	delete intBST_subtree8;
	return true;
}


int main(int argc, char** argv) 
{
	std::cout << "Progetto C++ Aprile Farjad Ali 829940" << std::endl << std::endl;
	std::cout << "Testing insert: \n" << (test1()?"PASS":"FAIL") << std::endl;
	std::cout << "Testing exist: \n" << (test2()?"PASS":"FAIL") << std::endl;
	std::cout << "Testing << and printIF: \n" << (test3()?"PASS":"FAIL") << std::endl;
	std::cout << "Test subtree: \n" << (test4()?"PASS":"FAIL") << std::endl;
	std::cout << "Test mixed: \n" << (test5()?"PASS":"FAIL") << std::endl;
}
