#ifndef BST_H
#define BST_H

template <typename T>
class BST
{
	private:
		long* elements;
		T* data;
		BST* father;
		BST* left;
		BST* right;
		
		void visit(std::ostream& os) const
		{
			if(data)
				os << *data << " ";
			if(left)
				left->visit(os);
			if(right)
				right->visit(os);
			
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
		
		BST(BST* _father)
		{
			elements = new long(0);
			data = nullptr;
			left = nullptr;
			right = nullptr;
			father = _father;
		}
		
		BST(T* _data)
		{
			elements = new long(0);
			father = nullptr;
			left = nullptr;
			right = nullptr;
			data = _data;
		}
		
		BST(T* _data, BST* _father)
		{
			elements = new long(0);
			left = nullptr;
			right = nullptr;
			data = _data;
			father = _father;
		}
		
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
			if(*_value > *data)
				if(right)
					right->insert(_value);
				else
				 {
				 	std::cout << "Added " << *_value << std::endl;
					right = new BST(_value, this);
				}
			else
				if(left)
					left->insert(_value);
				else
				{
					std::cout << "Added " << *_value << std::endl;
					left = new BST(_value, this);
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




























