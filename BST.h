#ifndef BST_H
#define BST_H

extern int new_count;
extern int delete_count;

template <typename T>
class BST
{
	public:
		typedef bool (*predicate)(const T&);
	
	private:
		long* elements;
		T* data;
		BST* father;
		BST* left;
		BST* right;
		
		static bool default_predicate(const T& elem)
		{
			return true;
		}
		
		void visit(std::ostream& os, predicate p = &default_predicate) const
		{
			if(data)
			{	
				if(!p)
					os << *data << " ";
				else if(p(*data))
					os << *data << " ";
					
			}
			if(left)
				left->visit(os, p);
			if(right)
				right->visit(os, p);
			
		}
		
	public:
		BST()
		{
			elements = new long(0);
			new_count++;
			data = nullptr;
			father = nullptr;
			left = nullptr;
			right = nullptr;
		}
		
		BST(T* _data)
		{
			elements = new long(0);
			new_count++;
			father = nullptr;
			left = nullptr;
			right = nullptr;
			data = _data;
		}
		
		BST(T* _data, BST* _father)
		{
			elements = new long(0);
			new_count++;
			left = nullptr;
			right = nullptr;
			data = _data;
			father = _father;
		}
		
		// Copy constructor
		BST(const BST& other)
			: father(nullptr)
			, data(new T(*other.data))
			, elements(new long(*other.elements))
		{
			new_count += 2;

			if (other.left)
			{
				this->left = new BST(*other.left);
				new_count++;
			}
				
			if (other.right)
			{
				this->right = new BST(*other.right);
				new_count++;
			}
		}
		
		~BST()
		{
			delete this->elements;
			delete this->data;
			delete this->left;
			delete this->right;
			
			delete_count += 4;
		}
		
		void insert(T* _value)
		{
			if (!data)
			{
				father = nullptr;
				left = nullptr;
				right = nullptr;
				data = _value;
				std::cout << "Added " << *_value << std::endl;
				return;
			}
			if(*_value > *data)
				if(right)
					right->insert(_value);
				else
				 {
				 	std::cout << "Added " << *_value << std::endl;
					right = new BST(_value, this);
					new_count++;
				}
			else
				if(left)
					left->insert(_value);
				else
				{
					std::cout << "Added " << *_value << std::endl;
					left = new BST(_value, this);
					new_count++;
				}
			(*elements)++;
		}
		
		bool exists(T key)
		{
			if (data && key==*data)
				return true;
			if (data)
			{
				if (key > *data && right)
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
		BST<T>* subtree(T key)
		{
			if (data && key==*data)
				return this;
			if (data)
			{
				if (key > *data && right)
					return right->subtree(key);
				else if (left)
					return left->subtree(key);
				return nullptr;
			}
			else 
			{
				if(right)
					return right->subtree(key);
				if(left)
					return left ->subtree(key);
				return nullptr;
			}
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
		
		template <typename Q>
		friend std::ostream& operator<< (std::ostream&, const BST<Q>&);
};

template<typename T>
std::ostream& operator<< (std::ostream& os, const BST<T>& elem)
{
	elem.visit(os);
	return os;
}

#endif




























