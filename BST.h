#ifndef BST_H
#define BST_H


template <typename T>
class BST
{
	public:
		typedef bool (*predicate)(const T&);
		typedef int  (*compareFunc)(const T&, const T&);
		
	private:
		compareFunc compFunc;
		long* elements;
		T* data;
		BST* father;
		BST* left;
		BST* right;
		BST* beg;
		
		static bool default_predicate(const T& elem)
		{
			return true;
		}
		
		static int default_compare_func(const T& elem1, const T& elem2) 
		{
			if(elem1 < elem2)
				return -1;
			else if(elem1 > elem2)
				return 1;
			else
				return 0;
		}
		
		void visit(std::ostream& os, predicate p = &default_predicate) const
		{
			if(left)
				left->visit(os, p);
			if(data)
			{	
				if(!p)
					os << *data << " ";
				else if(p(*data))
					os << *data << " ";		
			}
			if(right)
				right->visit(os, p);
		}
		
	public:
		BST(compareFunc f = &default_compare_func)
		{
			elements = new long(0);
			data = nullptr;
			father = nullptr;
			left = nullptr;
			right = this;
			compFunc = f;
		}
		
		BST(T* _data, compareFunc f = &default_compare_func)
		{
			elements = new long(0);
			father = nullptr;
			left = nullptr;
			right = nullptr;
			data = _data;
			compFunc = f;
		}
		
		BST(T* _data, BST* _father, compareFunc f = &default_compare_func)
		{
			elements = new long(0);
			left = nullptr;
			right = nullptr;
			data = _data;
			father = _father;
			compFunc = f;
		}
		
		//Copy constructor
		BST(const BST& other)
		{
			//std::cout << "COPY CONSTRUCTOR\n";
			this->compFunc = other.compFunc;
			this->father = nullptr;
			std::cout << *other.data << std::endl;
			this->data = new T(*other.data);
			this->elements = new long(*other.elements);
			
			if (other.left)
				this->left = new BST(*(other.left));
			else
				this->left = nullptr;
			if (other.right)
				this->right = new BST(*(other.right));
			else
				this->right = nullptr;
		}
		
		~BST()
		{
			delete this->elements; //Da decidere
			delete this->data;
			delete this->left;
			delete this->right;
		}
		
		//Getter and setters;
		
		
		void insert(T* _value)
		{
			if(!data)
			{
				father = nullptr;
				left = nullptr;
				right = nullptr;
				data = _value;
				std::cout << "Added " << *_value << std::endl;
				return;
			}
			if(compFunc(*_value, *data) > 0)
				if(right)
					right->insert(_value);
				else
				 {
				 	std::cout << "Added " << *_value << std::endl;
					right = new BST(_value, this);
					
				}
			else if(compFunc(*_value, *data) < 0)
				if(left)
					left->insert(_value);
				else
				{
					std::cout << "Added " << *_value << std::endl;
					left = new BST(_value, this);
					
				}
			else
				throw "Valore gia' presente!";
			(*elements)++;
		}
		
		bool exists(T key)
		{
			if (data && key==*data)
				return true;
			if (data)
			{
				if (compFunc(key, *data) > 0 && right)
					return right->exists(key);
				else if (left)
					return left->exists(key);
				return false;
			}
			else 
			{
				bool ret = false;
				if(right)
					ret = ret || right->exists(key);
				if(left)
					ret = ret || left ->exists(key);
				return ret;
			}
		}
		
		//TODO Restituire una copia anziche' una parte del BST
		BST* subtree(T key)
		{
			if(!data)
				std::cout << "Breach!\n";
			std::cout << "Visting: " << *(this->data) << std::endl;
			if (data && compFunc(key, *data) == 0)
				return new BST(*this);
			if (data)
			{
				if (compFunc(key, *data) > 0 && right)
					return right->subtree(key);
				else if (left)
					return left->subtree(key);
			}
			return nullptr;
		}
		
		BST<int>& operator=(const BST<int>& other)
		{
			//std::cout << "ASSIGNMENT OPERATOR\n";
			if(&other != this)
			{
				this->elements = other.elements;
				this->data = other.data;
				this->father = other.father;
				this->left = other.left;
				this->right = other.right;
			}
			return *this;
		}
		
		void printif(predicate p = default_predicate)
		{
			visit(std::cout, p);
			std::cout << std::endl;
		}
		
		void elems()
		{
			std::cout << "Got " << *(this->elements) << " items!" << std::endl;
		}
		
		class ConstBSTForwardIterator;
		
		template <typename Q>
		friend std::ostream& operator<< (std::ostream&, const BST<Q>&);
		
		class BSTForwardIterator 
		{		
			private:
				BST* pdata;
				bool leftdone;
			public:
				typedef std::forward_iterator_tag iterator_category;
				typedef T                         value_type;
				typedef ptrdiff_t                 difference_type;
				typedef T*                        pointer;
				typedef T&                        reference;
		
			
				BSTForwardIterator() : pdata(nullptr), leftdone(false) { }
				
				BSTForwardIterator(const BSTForwardIterator &other) : pdata(other.pdata), leftdone(other.leftdone) { }
		
				BSTForwardIterator& operator=(const BSTForwardIterator &other) {
					pdata = other.pdata;
					leftdone = other.leftdone;
					return *this;
				}
		
				~BSTForwardIterator() { }
		
				// Ritorna il dato riferito dall'iteratore (dereferenziamento)
				reference operator*() const {
					return *pdata->data;
				}
		
				// Ritorna il puntatore al dato riferito dall'iteratore
				pointer operator->() const {
					return pdata->data;
				}
		
				// Operatore di iterazione post-incremento
				BSTForwardIterator operator++(int) {
					BSTForwardIterator tmp(*this);
					next();
					return tmp;
				}
		
				// Operatore di iterazione pre-incremento
				BSTForwardIterator& operator++() {
					next();
					return *this;
				}
		
				// Uguaglianza
				bool operator==(const BSTForwardIterator &other) const {
					return pdata == other.pdata;
				}
		
				// Diversita'
				bool operator!=(const BSTForwardIterator &other) const {
					return pdata != other.pdata;
				}
				
				friend class ConstBSTForwardIterator;
		
				// Uguaglianza
				bool operator==(const ConstBSTForwardIterator &other) const {
					return pdata == other.pdata;
				}
		
				// Diversita'
				bool operator!=(const ConstBSTForwardIterator &other) const {
					return pdata != other.pdata;
				}
		
			private:
		
				// La classe container deve essere messa friend dell'iteratore per poter
				// usare il costruttore di inizializzazione.
				friend class BST<T>; // !!! Da cambiare il nome!
				
				void next()
				{
					if(!pdata)
						return;
					std::cout << "aaaaaaaaaaaaaCDATA: " << *pdata->data << std::endl; 
					// If left child is not traversed, find the 
			        // leftmost child 
			        if (!leftdone) 
			        { 
			            while (pdata->left) 
			                pdata = pdata->left;
			          	leftdone = true; 
			          	return;
			        }  
			        
			        // If right child exists 
			        if (pdata->right) 
			        { 
			            leftdone = false; 
			            pdata = pdata->right; 
			        } 
			  
			        // If right child doesn't exist, move to father 
			        else if (pdata->father) 
			        { 
			            // If this node is right child of its father, 
			            // visit father's father first 
			            while (pdata->father && 
			                   pdata == pdata->father->right) 
			                pdata = pdata->father; 
			            if (!pdata->father)
						{ 
			                pdata = nullptr;
			                //std::cout << "AFTER-CDATA: " << *pdata->data << std::endl; 
			                return;
			            }
			            pdata = pdata->father;
			        }
			        else
			        	pdata = nullptr;
			        
			        //std::cout << "AFTER-CDATA: " << *pdata->data << std::endl; 
				}
				
				// Costruttore privato di inizializzazione usato dalla classe container
				// tipicamente nei metodi begin e end
				explicit BSTForwardIterator(BST<T> *p) : pdata(p), leftdone(false) {next();}
						
		}; // classe BSTForwardIterator
		class ConstBSTForwardIterator 
		{
			const BST* pdata;	
			bool leftdone;
			public:
				typedef std::forward_iterator_tag iterator_category;
				typedef T                         value_type;
				typedef ptrdiff_t                 difference_type;
				typedef const T*                  pointer;
				typedef const T&                  reference;
		
			
				ConstBSTForwardIterator() : pdata(nullptr), leftdone(false) { }
				
				ConstBSTForwardIterator(const ConstBSTForwardIterator &other) : pdata(other.pdata), leftdone(false) { }
		
				ConstBSTForwardIterator& operator=(const ConstBSTForwardIterator &other) {
					pdata = other.pdata;
					leftdone = other.leftdone;
					return *this;
				}
		
				~ConstBSTForwardIterator() { }
		
				// Ritorna il dato riferito dall'iteratore (dereferenziamento)
				reference operator*() const {
					return *pdata->data;
				}
		
				// Ritorna il puntatore al dato riferito dall'iteratore
				pointer operator->() const {
					return pdata->data;
				}
				
				// Operatore di iterazione post-incremento
				ConstBSTForwardIterator operator++(int) {
					ConstBSTForwardIterator tmp(*this);
					next();
					return tmp;
				}
		
				// Operatore di iterazione pre-incremento
				ConstBSTForwardIterator& operator++() {
					next();
					return *this;
				}
		
				// Uguaglianza
				bool operator==(const ConstBSTForwardIterator &other) const {
					return pdata == other.pdata;
				}
				
				// Diversita'
				bool operator!=(const ConstBSTForwardIterator &other) const {
					return pdata != other.pdata;
				}
		
				friend class BSTForwardIterator;
		
				// Uguaglianza
				bool operator==(const BSTForwardIterator &other) const {
					return pdata == other.pdata;
				}
		
				// Diversita'
				bool operator!=(const BSTForwardIterator &other) const {
					return pdata != other.pdata;
				}
		
				// Costruttore di conversione BSTForwardIterator -> ConstBSTForwardIterator
				ConstBSTForwardIterator(const BSTForwardIterator &other) : pdata(other.pdata), leftdone(other.leftdone) {
				}
		
				// Assegnamento di un BSTForwardIterator ad un ConstBSTForwardIterator
				ConstBSTForwardIterator &operator=(const BSTForwardIterator &other) {
					pdata = other.pdata;
					leftdone = other.leftdone;
					return *this;
				}
	
			private:
	
				// La classe container deve essere messa friend dell'iteratore per poter
				// usare il costruttore di inizializzazione.
				friend class BST<T>; // !!! Da cambiare il nome!
				
				void next()
				{
					if(!pdata)
					{
						//std::cout << "BREACH\n";
						return;
					}
					//std::cout << "CDATA: " << *pdata->data << std::endl; 
					// If left child is not traversed, find the 
			        // leftmost child 
			        if (!leftdone) 
			        { 
			        	leftdone = true;
			        	if(pdata->left)
			        	{
			            	while (pdata->left) 
			                	pdata = pdata->left;
							return;
						}
			        }  
			        
			        // If right child exists 
			        if (pdata->right) 
			        { 
			            leftdone = false; 
			            pdata = pdata->right;
				        if (!leftdone) 
				        { 
				        	leftdone = true;
				            while (pdata->left) 
				                pdata = pdata->left;
				        }  
			            return;
			        } 
			  		
			  		
			        // If right child doesn't exist, move to father 
			        else if (pdata->father) 
			        { 
			            // If this node is right child of its father, 
			            // visit father's father first 
			            while (pdata->father && 
			                   pdata == pdata->father->right) 
			                pdata = pdata->father; 
			            if (!pdata->father)
						{ 
			                pdata = nullptr;
			                //std::cout << "AFTER-CDATA: " << *pdata->data << std::endl; 
			                return;
			            }
			            pdata = pdata->father;
			        }
			        else
			        	pdata = nullptr;
			        
			        //std::cout << "AFTER-CDATA: " << *pdata->data << std::endl; 
				}
				
		
				// Costruttore privato di inizializzazione usato dalla classe container
				// tipicamente nei metodi begin e end
				ConstBSTForwardIterator(const BST<T>* p) : pdata(p), leftdone(false) {next();}
				// !!! Eventuali altri metodi privati
				
		}; // classe ConstBSTForwardIterator
		
		public:
			
			typedef BSTForwardIterator iterator;
			typedef ConstBSTForwardIterator const_iterator;
						
			const_iterator begin()
			{
				return const_iterator(this);
			}
			
			const_iterator end()
			{
				return const_iterator(nullptr);
			}
};

template<typename T>
std::ostream& operator<< (std::ostream& os, const BST<T>& elem)
{
	elem.visit(os);
	return os;
}

#endif




























