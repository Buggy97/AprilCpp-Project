#ifndef BST_H
#define BST_H
#include <exception>
#include <iostream>
#include <iterator> //per std::forward_iterator_tag
#include <cstddef>  //per std::ptrdiff_t

/**
@file BST.h 
@brief Dichiarazione della classe BST (Binary search tree).
**/

//Forward declaration per operatore << 
template <typename T, typename Comparator> class BST;

//Per evitare che vengano create classi BST per puntatori
template <class T, typename Comparator>
class BST<T*, Comparator>
{
	private:
		BST(){}		
};

/**
@brief Eccezione per elemento doppio inserito.
**/
struct BSTDoubleElementException : public std::exception
{
	const char * what () const throw ()
    {
    	return "Tried to insert a duplicate element";
    }
};

/**
@brief Albero di ricerca binario (BST).

Classe che rappresenta un albero binario di ricerca di dati generici.
La struttura non e' autobilanciante, spetta all'utente un inserimento appropriato
per garantire le massime prestazioni di ricerca. La classe puo' essere costruita per 
dati che non sono puntatori e per cui e' possibile definire un Comparator, nel caso 
si provasse a instanziare per un dato di tipo puntatore si otterrebbe un errore di 
compilazione. La classe non effetua alcun deallocamento dei dati salvati all'interno
del ciascun nodo
Sono stati implementati gli iteratori di tipo const forward_iterator
@param T tipo del dato
@param Comparatore funtore di comparazione (<, >, =) di due dati, dato A e B resituisce 1 se A > B, -1 se A < B, 0 se A == B
**/
template <typename T, typename Comparator>
class BST
{				
	private:
		Comparator comparator; ///< Comparator per confrontare 2 elementi A e B
		long* elements; ///< Conteggio dei discendenti totali del nodo includendo se stesso
		T data; ///< Puntatore al dato del nodo
				 ///< Ogni nodo non alloca alcuna memoria per il dato in se
				 ///< in quanto salva l'indirizzo del dato, cio' consente 
				 ///< all'utente di gestire il ciclo di vita dei dati salvati
		BST* father; ///< Padre del nodo
		BST* left; ///< Figlio sinistro del nodo
		BST* right; ///< Figlio destro del nodo
		
		/**
		@brief Costruttore di copia interno.

		Costruttore di copia interno che permette di copiare un BST riassegnandoli il padre
		Viene usato internamente dal costruttore di copia pubblico
		@param other BST da usare per creare quello corrente con padre diverso
		@param _father padre per il nuovo BST
		**/
		BST(const BST<T, Comparator>& other, BST<T, Comparator>* _father)
		{
			this->father = _father;
			this->data = other.data;
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
		
	public:
		/**
		@brief Costruttore di default.

		Costruttore di default per istanziare un BST vuoto. E' l'unico costruttore
		che puo' essere usato per creare un BST vuoto
		**/
		BST():
			elements(new long(0)),
			father(nullptr),
			left(nullptr),
			right(nullptr)
		{
		}
		/**
		@brief Costruttore secondario.

		Costruttore che permette di instanziare un BST con un elemento
		**/
		BST(T& _data, BST* _father):
			elements(new long(1)),
			data(_data),
			father(_father),
			left(nullptr),
			right(nullptr)
		{
		}

		/**
		@brief Copy constructor.
		Copy constructor. Permette di istanziare un BST a partire da un'altro BST.
		La copia avviene in maniera ricorsiva per i livelli inferiori ed il nodo 
		corrente diventa il nuovo padre.
		@param other BST da usare per creare quello corrente
		**/
		BST(const BST<T, Comparator>& other)
		{
			this->father = nullptr;
			this->data = other.data;
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
		@brief Distruttore.
		Rimuove la memoria allocata dal BST.
		Il dato del nodo non viene eliminato poiche' e' una reference
		**/
		~BST()
		{
			delete this->elements;
			delete this->left;
			delete this->right;
		}
		/**
		@brief Nodi presenti nel BST.
		Restituisce il numero di nodi discendenti presenti nell'albero BST
		@return Numero di elementi dell'albero BST
		**/
		long total_elements() const
		{
			return *this->elements;
		}
		/**
		@brief Visita in order dell'albero e ne stampa il contenuto.
		Visita l'albero mediante l'iteratore della classe e stampa il contenuto
		di tutti gli elementi.
		@param os stream di output su cui scrivere il contenuto
		**/
		std::ostream& print(std::ostream& os = std::cout) const
		{
			ConstBSTForwardIterator it;
			it = this->begin();
			while(it!=this->end())
				os << *it << " ", it++;
			os << std::endl;
			return os;
		}
		/**
		@brief Inserisce un dato nel BST.
		L'inserimento avviene in maniera ricorsiva in modo da mantenere la proprieta'
		BST. Poiche' i valori uguali non sono supportati, lancia un'eccezione nel caso 
		si provasse ad inserire un valore gia' presente
		@param _value dato da inserire del tipo specificato alla definizione del BST
		**/
		void insert(T& _value)
		{
			(*elements)++;
			if(*this->elements==0) //BST vuoto
			{
				father = nullptr;
				left = nullptr;
				right = nullptr;
				data = _value;
				std::cout << "Added " << _value << std::endl;
				return;
			}
			if(comparator(_value, data) > 0)
				if(right)
					right->insert(_value);
				else
				 {
				 	std::cout << "Added " << _value << std::endl;
					right = new BST(_value, this);
					
				}
			else if(comparator(_value, data) < 0)
				if(left)
					left->insert(_value);
				else
				{
					std::cout << "Added " << _value << std::endl;
					left = new BST(_value, this);
					
				}
			else
				throw BSTDoubleElementException();
		}
	
		/**
		@brief Controlla se un elemento e' presente nel BST.
		Il controllo avviene in maniera ricorsiva. E' necessario
		all'utente per controllare se il valore che sta per inserire 
		e' gia' presente
		@param key valore da cercare nel BST
		@return Se e' presente o meno l'emento (boolean)
		**/
		bool exists(const T& key) const
		{
			if (*this->elements==0 && key==*data)
				return true;
			if (*this->elements==0)
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
		@brief Restituisce un sottoalbero avente come radice un nodo specifico.
		Trova un nodo con valore specificato e resitituisce una copia del suo
		sottoalbero.
		@param key dato della radice del sottoalbero
		@return Riferimento al nuovo sottoalbero, null se il valore non è presente
		**/
		BST* subtree(const T& key) const
		{
			if (*this->elements!=0 && comparator(key, data)==0)
				return new BST(*this, nullptr);
			if (*this->elements!=0)
			{
				if (comparator(key, data) > 0 && right)
					return right->subtree(key);
				else if (left)
					return left->subtree(key);
			}
			else
				return nullptr;
		}
		/**
		@brief Operatore di assegnamento.
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
	@brief Iteratore costante per albero BST.
	
	Classe iteratore per iterare sui nodi di un albero BST.
	L'iterazione avviene in order.
	**/
	class ConstBSTForwardIterator 
	{
		private:
			const BST<T, Comparator>* node; ///< Puntatore al nodo corrente
			bool leftdone; ///< Puntatore al dato del nodo
			
		public:
			typedef std::forward_iterator_tag iterator_category; ///< Trait stl
			typedef T                         value_type; ///< Trait stl
			typedef ptrdiff_t                 difference_type; ///< Trait stl
			typedef const T*                  pointer; ///< Trait stl
			typedef const T&                  reference; ///< Trait stl
	
			/**
			@brief Costruttore di default.
			Costruttore di default per istanziare un iteratore nullo.
			**/
			ConstBSTForwardIterator() : node(nullptr), leftdone(false) { }
			
			/**
			@brief Copy constructor.
		
			Costruttore di copia. Permette di istanziare un ConstBSTForwardIterator a partire da un'altro ConstBSTForwardIterator. 
			@param other ConstBSTForwardIterator da usare per creare quello corrente
			**/
			ConstBSTForwardIterator(const ConstBSTForwardIterator &other) : node(other.node), leftdone(false) { }
			
			/**
			@brief Operatore di assegnamento.
			Permette la copia tra ConstBSTForwardIterator.
			@param other iteratore sorgente
			@return riferimento a this
			**/
			ConstBSTForwardIterator& operator=(const ConstBSTForwardIterator &other) {
				node = other.node;
				leftdone = other.leftdone;
				return *this;
			}
			
			/**
			@brief Distruttore default.
			**/
			~ConstBSTForwardIterator() { }
	
			/** 
			@brief Ritorna il dato riferito dall'iteratore (dereferenziamento).
			**/
			reference operator*() const 
			{
				return node->data;
			}
			
			/**
			@brief Ritorna il puntatore al dato riferito dall'iteratore.
			**/
			pointer operator->() const 
			{
				return &node->data;
			}
			
			/**
			@brief Operatore di iterazione post-incremento.
			**/
			ConstBSTForwardIterator operator++(int)
			{
				ConstBSTForwardIterator tmp(*this);
				next();
				return tmp;
			}
			
			/**
			@brief Operatore di iterazione pre-incremento.
			**/
			ConstBSTForwardIterator& operator++()
			{
				next();
				return *this;
			}
			
			/**
			@brief Uguaglianza.
			**/
			bool operator==(const ConstBSTForwardIterator &other) const
			{
				return node == other.node;
			}
			
			/**
			@brief Diversita'.
			**/
			bool operator!=(const ConstBSTForwardIterator &other) const 
			{
				return node != other.node;
			}

		private:

			// La classe container deve essere messa friend dell'iteratore per poter
			// usare il costruttore di inizializzazione.
			friend class BST<T, Comparator>; 
			
			/**
			@brief Itera al prossimo nodo (in order).
			Itera in order al prossimo nodo dell'albero.
			Ogni qualvolta che viene chiamato l'operatore ++, viene chiamato
			questo metodo.
			**/
			void next()
			{
				if(!node)
					return;
				/* Se non abbiamo ancora trovato il figlio leftmost
		           continuiamo ad iterare
		        */
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
		        // Se il figlio destro esiste
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
		        // Se il figlio destro non esiste torno al padre
		        else if (node->father) 
		        { 
		            // Se questo nodo è il figlio del proprio padre
		            // allora visito il padre
		            while (node->father && 
		                   node == node->father->right) 
		                node = node->father; 
		            if (!node->father)
					{ 
		                node = nullptr;
		                return;
		            }
		            node = node->father;
		        }
		        else
		        	node = nullptr; 
			}
			
			/** 
			@brief Costruttore privato.
			Costruttore di inizializzazione usato dalla classe container.
			per metodi begin() e end()
			**/
			ConstBSTForwardIterator(const BST<T, Comparator>* p) : node(p), leftdone(false) {next();}
			
	};
	
	public:
		//Typedef
		typedef ConstBSTForwardIterator const_iterator;
		
		/** 
		@brief Restituisce un'iteratore che punta al primo elemento.
		@return iteratore che itera sull'albero in order
		**/		
		const_iterator begin() const
		{
			return const_iterator(this);
		}
		
		/** 
		@brief Restituisce un'iteratore che indica la fine.
		@return iteratore che indica la fine dell'iterazione
		**/
		const_iterator end() const
		{
			return const_iterator(nullptr);
		}
};

template<typename T, typename Comparator>
std::ostream& operator<< (std::ostream& os,const BST<T,Comparator>& elem)
{
	elem.print(os);
	return os;
}

#endif
