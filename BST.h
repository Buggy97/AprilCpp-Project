#ifndef BST_H
#define BST_H
#include <iostream>
#include <iterator> //per std::forward_iterator_tag
#include <cstddef>  //per std::ptrdiff_t

/**
@file BST.h 
@brief Dichiarazione della classe BST (Binary search tree)
**/

//Forward declaration per operatore << 
template <typename T, typename Comparator> class BST;
template <typename T, typename Comparator> std::ostream& operator<<( std::ostream&, const BST<T, Comparator>& );

//Per evitare che vengano create classi BST per puntatori
template <class T, typename Comparator>
class BST<T*, Comparator>
{
	private:
		BST(){}		
};

/**
@brief Albero di ricerca binario (BST)

Classe che rappresenta un albero binario di ricerca di dati generici
La struttura non e' autobilanciante, spetta all'utente un inserimento appropriato
per garantire le massime prestazioni di ricerca. La classe puo' essere costruita per 
dati che non sono puntatori e per cui e' possibile definire un Comparator, nel caso 
si provasse a instanziare per un dato di tipo puntatore si otterrebbe un errore di 
compilazione. La classe non effetua alcun deallocamento dei dati salvati all'interno
del ciascun nodo
Sono stati implementati gli iteratori di tipo const forward_iterator
**/
template <typename T, typename Comparator>
class BST
{				
	private:
		Comparator comparator; ///< Comparator per confrontare 2 elementi A e B
		long* elements; ///< Conteggio dei discendenti totali del nodo includendo se stesso
		T* data; ///< Puntatore al dato del nodo
				///< Ogni nodo non alloca alcuna memoria per il dato in se
				///< in quanto salva l'indirizzo del dato, cio' consente 
				///< all'utente di gestire il ciclo di vita dei dati salvati
		BST* father; ///< Padre del nodo
		BST* left; ///< Figlio sinistro del nodo
		BST* right; ///< Figlio destro del nodo
		
	public:
		/**
		@brief Costruttore di default

		Costruttore di default per istanziare un BST vuoto. E' l'unico costruttore
		che puo' essere usato per creare un BST vuoto
		**/
		BST():
			elements(new long(0)),
			data(nullptr),
			father(nullptr),
			left(nullptr),
			right(nullptr)
		{
		}
		/**
		@brief Costruttore secondario

		Costruttore che permette di instanziare un BST con un elemento
		**/
		BST(T& _data, BST* _father):
			elements(new long(1)),
			data(& _data),
			father(_father),
			left(nullptr),
			right(nullptr)
		{
		}
		/**
		@brief Costruttore di copia interno

		Costruttore di copia interno che permette di copiare un BST riassegnandoli il padre
		Viene usato internamente dal costruttore di copia pubblico
		@param other BST da usare per creare quello corrente con padre diverso
		@param _father padre per il nuovo BST
		**/
		BST(const BST<T, Comparator>& other, BST<T, Comparator>* _father)
		{
			this->father = _father;
			this->data = *other.data;
			this->elements = new long(*other.elements);
			if (other.left)
				this->left = new BST(*(other.left), _father);
			else
				this->left = nullptr;
			if (other.right)
				this->right = new BST(*(other.right), _father);
			else
				this->right = nullptr;
		}
		/**
		@brief Copy constructor
	
		Costruttore di copia. Permette di istanziare un BST a partire da un'altro BST.
		La copia avviene in maniera ricorsiva 
		@param other BST da usare per creare quello corrente
		**/
		BST(const BST<T, Comparator>& other) //La copy constructor copia dall'alto verso in basso perche' se copiasse anche il padre rischierebbe di non mantenere la proprieta' BST
		{
			this->father = nullptr;
			this->data = *other.data;
			this->elements = new long(*other.elements);
			
			if (other.left)
				this->left = new BST(*(other.left), this);
			else
				this->left = nullptr;
			if (other.right)
				this->right = new BST(*(other.right), this);
			else
				this->right = nullptr;
		}
		/**
		@brief Distruttore
		Rimuove la memoria allocata dal BST
		Il dato del nodo non viene eliminato poiche' e' una reference
		**/
		~BST()
		{
			delete this->elements;
			delete this->left;
			delete this->right;
		}
		/**
		@brief Inserisce un dato nel BST 
		L'inserimento avviene in maniera ricorsiva in modo da mantenere la proprieta'
		BST. Poiche' i valori uguali non sono supportati, lancia un'eccezione nel caso 
		si provasse ad inserire un valore gia' presente
		@param _value dato da inserire del tipo specificato alla definizione del BST
		**/
		void insert(T& _value)
		{
			if(!data) //BST vuoto
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
				std::cout << "Added " << _value << std::endl; //Lancia eccezione

			(*elements)++;
		}
		/**
		@brief Controlla se un elemento e' presente nel BST
		Il controllo avviene in maniera ricorsiva. E' necessario
		all'utente per controllare se il valore che sta per inserire 
		e' gia' presente
		@param key valore da cercare nel BST
		@return Se e' presente o meno l'emento (boolean)
		**/
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
		
		/**
		@brief Restituisce un sottoalbero avente come radice un nodo specifico
		Trova un nodo con valore specificato e resitituisce una copia del suo
		sottoalbero.
		@param key dato della radice del sottoalbero
		@return Riferimento al nuovo sottoalbero
		**/
		BST* subtree(const T& key)
		{
			if (data && comparator(key, *data) == 0)
				return new BST(*this);
			if (data)
			{
				if (comparator(key, *data) > 0 && right)
					return right->subtree(key);
				else if (left)
					return left->subtree(key);
			}
			if(!this->left && !this->right)
				throw "Not found";
		}
		
		/**
		@brief Operatore di assegnamento
		Permette la copia tra BST
		@param other albero sorgente
		@return riferimento a this
		**/
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
	
	//Dichiarazione friend per printIF			
	template <typename Q, typename G, typename Predicate> 
	friend void printIF(BST<Q, G>* tree);
	
	/**
	@brief Iteratore costante per albero BST
	
	Classe iteratore per iterare sui nodi di un albero BST.
	L'iterazione avviene in order.
	**/
	class ConstBSTForwardIterator 
	{
		private:
			const BST<T, Comparator>* node;
			bool leftdone;
		public:
			typedef std::forward_iterator_tag iterator_category;
			typedef T                         value_type;
			typedef ptrdiff_t                 difference_type;
			typedef const T*                  pointer;
			typedef const T&                  reference;
	
		
			ConstBSTForwardIterator() : node(nullptr), leftdone(false) { }
			
			ConstBSTForwardIterator(const ConstBSTForwardIterator &other) : node(other.node), leftdone(false) { }
	
			ConstBSTForwardIterator& operator=(const ConstBSTForwardIterator &other) {
				node = other.node;
				leftdone = other.leftdone;
				return *this;
			}
	
			~ConstBSTForwardIterator() { }
	
			// Ritorna il dato riferito dall'iteratore (dereferenziamento)
			reference operator*() const {
				return *node->data;
			}
	
			// Ritorna il puntatore al dato riferito dall'iteratore
			pointer operator->() const {
				return node->data;
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
				return node == other.node;
			}
			
			// Diversita'
			bool operator!=(const ConstBSTForwardIterator &other) const {
				return node != other.node;
			}

		private:

			// La classe container deve essere messa friend dell'iteratore per poter
			// usare il costruttore di inizializzazione.
			friend class BST<T, Comparator>; 
			
			void next()
			{
				if(!node)
				{
					//std::cout << "BREACH\n";
					return;
				}
				//std::cout << "CDATA: " << *node->data << std::endl; 
				// If left child is not traversed, find the 
		        // leftmost child 
		        if (!leftdone) 
		        { 
		        	leftdone = true;
		        	if(node->left)
		        	{
		            	while (node->left) 
		                	node = node->left;
						return;
					}
		        }  
		        
		        // If right child exists 
		        if (node->right) 
		        { 
		            leftdone = false; 
		            node = node->right;
			        if (!leftdone) 
			        { 
			        	leftdone = true;
			            while (node->left) 
			                node = node->left;
			        }  
		            return;
		        } 
		  		
		  		
		        // If right child doesn't exist, move to father 
		        else if (node->father) 
		        { 
		            // If this node is right child of its father, 
		            // visit father's father first 
		            while (node->father && 
		                   node == node->father->right) 
		                node = node->father; 
		            if (!node->father)
					{ 
		                node = nullptr;
		                //std::cout << "AFTER-CDATA: " << *node->data << std::endl; 
		                return;
		            }
		            node = node->father;
		        }
		        else
		        	node = nullptr;
		        
		        //std::cout << "AFTER-CDATA: " << *node->data << std::endl; 
			}
			
	
			// Costruttore privato di inizializzazione usato dalla classe container
			// tipicamente nei metodi begin e end
			ConstBSTForwardIterator(const BST<T, Comparator>* p) : node(p), leftdone(false) {next();}
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
	if(elem.left)
		os << elem.left;
	if(elem.data)
		os << *elem.data << " ";	
	if(elem.right)
		os << elem.left;
	return os;
}

#endif


