#ifndef BST_H
#define BST_H
#include <exception> //per usare eccezioni
#include <iostream>  //per usare cout
#include <iterator>  //per std::forward_iterator_tag
#include <cstddef>   //per std::ptrdiff_t
#include <algorithm>  //per std::swap

/**
@file BST.h 
@brief Dichiarazione della classe BST (Binary search tree).
**/

/**
@brief Eccezione per elemento doppio inserito.
**/
class BSTDoubleElementException : public std::exception
{
	public:
		virtual const char* what() const throw ()
	    {
	    	return "cannot insert duplicate item in BST";
	    }
};

/**
@brief Funtore di comparazione di default della classe.

Funtore di comparazione default dell'albero. 
Dati due oggetti A e B restituisce >0 se A>B, <0 se A<B, 0 se A==B
@param T il tipo dell'oggeto
**/
template <typename T>
struct _BSTdefault_comparator
{
	inline int operator()(const T& a, const T& b) const
	{
		if (a > b)
			return  1;
		else if (a < b)
			return -1;
		else
			return 0;
	}
};

/**
@brief Albero di ricerca binario (BST).

Classe che rappresenta un albero binario di ricerca di dati generici.
La struttura non e' autobilanciante, spetta all'utente un inserimento appropriato
per garantire le massime prestazioni di ricerca. Sono stati implementati
gli iteratori di tipo const forward iterator
@param T tipo del dato
@param Comparatore funtore di comparazione (<, >, =) di due dati, dato A e B resituisce >0 se A > B, <0 se A < B, 0 se A == B
	   se non specificato viene usato quello di default. @see _BSTdefault_comparator
**/
template <typename T, typename Comparator = _BSTdefault_comparator<T> >
class BST
{				
	private:
	    static Comparator comparator; ///< Comparator per confrontare 2 elementi A e B
		long* elements; ///< Conteggio dei discendenti totali del nodo includendo se stesso
		T* data; ///< Puntatore al dato del nodo 
		BST* father; ///< Padre del nodo
		BST* left; ///< Figlio sinistro del nodo
		BST* right; ///< Figlio destro del nodo

		/**
		@brief Costruttore secondario.

		Costruttore che permette di instanziare un BST con un elemento.
		Viene usato dal operazione di inserimento.
		**/
		BST(const T& _data, BST* _father):
			elements(new long(1)),
			data(new T(_data)),
			father(_father),
			left(nullptr),
			right(nullptr)
		{
		}

	public:
		/**
		@brief Costruttore di default.

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
		@brief Copy constructor.
		Copy constructor. Permette di istanziare un BST a partire da un'altro BST.
		La copia avviene in maniera ricorsiva per i livelli inferiori ed il nodo 
		corrente diventa il nuovo padre.
		@param other BST da usare per creare quello corrente
		**/
		BST(const BST<T, Comparator>& other)
		{
			this->father = nullptr;
			if(other.data)
				this->data = new T(*other.data);
			else
				this->data = nullptr;
			this->elements = new long(*other.elements);
			if (other.left)
			{
				this->left = new BST(*(other.left));
				this->left->father = this;
			}
			else
				this->left = nullptr;
			if (other.right)
			{
				this->right = new BST(*(other.right));
				this->right->father = this;
			}
			else
				this->right = nullptr;
		}
		/**
		@brief Distruttore.
		Rimuove la memoria allocata dal BST.
		**/
		~BST()
		{
			delete this->elements;
			delete this->data;
			delete this->left;
			delete this->right;
		}
		/**
		@brief Nodi presenti nel BST.
		Restituisce il numero di nodi discendenti presenti nell'albero BST
		@return Numero di elementi dell'albero BST
		**/
		long size() const
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
			ConstBSTForwardIterator it = this->begin();;
			ConstBSTForwardIterator it_end = this->end();
			while(it!=it_end)
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
		void insert(const T& _value)
		{
			if(*this->elements==0) //BST vuoto
			{	
				father = nullptr;
				left = nullptr;
				right = nullptr;
				data = new T(_value);
				(*elements)++;
				return;
			}
			if(comparator(_value, *data) > 0)
			{
				if(right)
					right->insert(_value);
				else
					right = new BST(_value, this);
			}
			else if(comparator(_value, *data) < 0)
			{
				if(left)
					left->insert(_value);
				else
					left = new BST(_value, this);
			}
			else if(comparator(_value, *data) == 0)
				throw BSTDoubleElementException();
			else
				return;
			(*elements)++;
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
			if(*this->elements==0) //Caso in cui BST e' vuoto
				return false;
			if(comparator(key, *data)==0) //Ho trovato l'elemento
				return true;
			else if(comparator(key, *data) > 0) //Maggiore del nodo corrente
			{
				if(this->right)
					return this->right->exists(key);
			}
			else if(comparator(key, *data) < 0) //Minore del nodo corrente
				if(this->left)
					return this->left->exists(key);
			return false;
		}
		
		/**
		@brief Restituisce un sottoalbero avente come radice un nodo specifico.
		Trova un nodo con valore specificato e resitituisce una copia del suo
		sottoalbero.
		@param key dato della radice del sottoalbero
		@return Riferimento al nuovo sottoalbero, null se il valore non � presente
		**/
		BST<T, Comparator>* subtree(const T& key) const
		{
			if (*this->elements!=0 && comparator(key, *data)==0)
				return new BST(*this);
			else if (*this->elements!=0)
			{
				if (comparator(key, *data) > 0 && right) //Maggiore del nodo corrente
					return right->subtree(key);		
				else if (left)							//Minore del nodo corrente
					return left->subtree(key);
			}
			return nullptr;							//Non ho trovato nulla
		}		
		
		/**
		@brief Operatore di assegnamento.
		Permette la copia tra BST
		@param other albero sorgente
		@return riferimento a this
		**/
		BST<T, Comparator>& operator=(const BST<T, Comparator>& other)
		{
			if(&other != this)
			{
				//Copio other in oggetto temporaneo ed effettuo lo swap sui membri
				//in questo modo i membri di questo oggetto verranno deallocati alla
				//uscita dello scope
				BST<T, Comparator> other_tmp(other);
				std::swap(this->father, other_tmp.father);
				std::swap(this->data, other_tmp.data);
				std::swap(this->left, other_tmp.left);
				std::swap(this->right, other_tmp.right);
				std::swap(this->elements, other_tmp.elements);
			}
			return *this;
		}
		
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
			@return l'oggetto iterato
			**/
			reference operator*() const 
			{
				return *node->data;
			}
			
			/**
			@brief Ritorna il puntatore al dato riferito dall'iteratore.
			@return puntatore dell'oggetto iterato
			**/
			pointer operator->() const 
			{
				return node->data;
			}
			
			/**
			@brief Operatore di iterazione post-incremento.
			@return riferimento dell'iteratore
			**/
			ConstBSTForwardIterator operator++(int)
			{
				ConstBSTForwardIterator tmp(*this);
				next();
				return tmp;
			}
			
			/**
			@brief Operatore di iterazione pre-incremento.
			@return riferimento dell'iteratore
			**/
			ConstBSTForwardIterator& operator++()
			{
				next();
				return *this;
			}
			
			/**
			@brief Uguaglianza.
			@param elemento da confrontare
			@return booleano che indica l'esito
			**/
			bool operator==(const ConstBSTForwardIterator &other) const
			{
				return node == other.node;
			}
			
			/**
			@brief Diversita'.
			@param elemento da confrontare
			@return booleano che indica l'esito
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
					}
					return;
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
		            // Se questo nodo � il figlio del proprio padre
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
			ConstBSTForwardIterator(const BST<T, Comparator>* p) : node(p), leftdone(false)
			{
				next();
			}
			
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

/**
	@brief Inizializza il funtore di comparazione statico.
		   Questo permette di avere un'unica istanza del comparatore
		   per classe.
**/
template<typename T, typename Comparator> Comparator BST<T, Comparator>::comparator;

/**
	@brief Overloading dell'operatore <<.
		   Permette di stampare il contenuto in order dell'albero.
	@param os std::ostream su cui stampare il contenuto
	@param elem elemento da stampare
	@return riferimento della std::stream su cui e' stato stampato il contenuto
**/
template<typename T, typename Comparator>
std::ostream& operator<< (std::ostream& os, const BST<T,Comparator>& elem)
{
	elem.print(os);
	return os;
}
/**
	@brief Funzione globale printIF
	Funzione che stampa il contenuto dell'albero BST
	che soddisfra un particolare predicato.
	@param T tipo del BST
	@param Comparator scomparatore usato dal bst
	@param Predicate predicato che un dato deve soddisfare per essere stampato
**/
template <typename T, typename Predicate> 
void printIF(const T& tree)
{
	Predicate predicate;
	typename T::const_iterator it = tree.begin();
	typename T::const_iterator it_end = tree.end();
	while(it!=it_end)
	{
		if(predicate(*it))
			std::cout << *it << " ";
		it++;
	}
}
#endif
