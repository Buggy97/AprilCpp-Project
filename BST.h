#ifndef BST_H
#define BST_H

//Forward declaration per operatore << 
template <typename T, typename Comparator> class BST;
template <typename T, typename Comparator> std::ostream& operator<<( std::ostream&, const BST<T, Comparator>& );

//Per evitare che vengano create classi BST per puntatori.
template <class T, typename Comparator>
class BST<T*, Comparator>
{
	private:
		BST(){}		
};

template <typename T, typename Comparator>
class BST
{
	public:
		typedef bool (*predicate)(const T&);
				
	private:
		Comparator comparator;
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
		BST()
		{
			elements = new long(0);
			data = nullptr;
			father = nullptr;
			left = nullptr;
			right = nullptr;
		}
		
		BST(T& _data)
		{
			elements = new long(0);
			father = nullptr;
			left = nullptr;
			right = nullptr;
			data = &_data;
		}
		
		BST(T& _data, BST* _father)
		{
			elements = new long(0);
			left = nullptr;
			right = nullptr;
			data = &_data;
			father = _father;
		}
		
		//Copy constructor
		BST(const BST<T, Comparator>& other)
		{
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
			delete this->elements;
			delete this->left;
			delete this->right;
		}
		
		//Getter and setters;
		
		
		void insert(T& _value)
		{
			if(!data)
			{
				father = nullptr;
				left = nullptr;
				right = nullptr;
				data = &_value;
				std::cout << "Added " << _value << std::endl;
				return;
			}
			if(comparator(_value, *data) > 0)
				if(right)
					right->insert(_value);
				else
				 {
				 	std::cout << "Added " << _value << std::endl;
					right = new BST(_value, this);
					
				}
			else if(comparator(_value, *data) < 0)
				if(left)
					left->insert(_value);
				else
				{
					std::cout << "Added " << _value << std::endl;
					left = new BST(_value, this);
					
				}
			else
				std::cout << "Added " << _value << std::endl;

			(*elements)++;
		}
		
		bool exists(T& key)
		{
			if (data && key==*data)
				return true;
			if (data)
			{
				if (comparator(key, *data) > 0 && right)
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
		BST* subtree(const T& key)
		{
			if(!data)
				std::cout << "Breach!\n";
			std::cout << "Visting: " << *(this->data) << std::endl;
			if (data && comparator(key, *data) == 0)
				return new BST(*this);
			if (data)
			{
				if (comparator(key, *data) > 0 && right)
					return right->subtree(key);
				else if (left)
					return left->subtree(key);
			}
			return nullptr;
		}
		
		//Assegnamento solo tra BST con stesso comparator e stesso tipi di dati
		BST<T, Comparator>& operator=(const BST<T, Comparator>& other)
		{
			//std::cout << "ASSIGNMENT OPERATOR\n";
			if(&other != this)
			{
				this->elements = other.elements;
				this->data = other.data;
				this->father = other.father;
				this->left = other.left;
				this->right = other.right;
				this->beg = other.beg;
			}
			return *this;
		}
	
		
		void elems()
		{
			std::cout << "Got " << *(this->elements) << " items!" << std::endl;
		}

		friend std::ostream& operator<< <T, Comparator>( std::ostream&, const BST<T, Comparator>& );
		
		template <typename Q, typename G, typename Predicate> 
		friend void printIF(BST<Q, G>* tree);
		
		class ConstBSTForwardIterator 
		{
			private:
				const BST<T, Comparator>* pdata;	
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
	
			private:
	
				// La classe container deve essere messa friend dell'iteratore per poter
				// usare il costruttore di inizializzazione.
				friend class BST<T, Comparator>; // !!! Da cambiare il nome!
				
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
				ConstBSTForwardIterator(const BST<T, Comparator>* p) : pdata(p), leftdone(false) {next();}
				// !!! Eventuali altri metodi privati
				
		}; // classe ConstBSTForwardIterator
		
		public:
			
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

template<typename T, typename Comparator>
std::ostream& operator<< (std::ostream& os, const BST<T, Comparator>& elem)
{
	elem.visit(os);
	return os;
}

#endif


