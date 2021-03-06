using namespace std;

template<class T>
class Node{
	private:
		T* data;
		Node* lchild;
		Node* rchild;

	public:
		Node(){data=new T;lchild=NULL;rchild=NULL;};
		Node(T indata){data=new T;*data=indata;lchild=NULL;rchild=NULL;};
		~Node(){delete data;		//child nodes reach end of lifetime, and should be getting deleted
		
		};


		void set_data(T input){*data=input;};
		void set_lchild(T input);
		void set_rchild(T input);
	
		T get_data(){return *data;};
		T* get_dataAdd(){return data;};
		Node<T>* get_lchild(){return lchild;};
		Node<T>* get_rchild(){return rchild;};
};

template<class T>
void Node<T>::set_lchild(T input){
	lchild=new Node<T>(input);
}

template<class T>
void Node<T>::set_rchild(T input){
	rchild=new Node<T>(input);
}

template<class T>
class BST{
	
	protected:
		Node<T>* root;

	public:
		BST(){root=new Node<T>();};
		BST(T rootData){root=new Node<T>(rootData);};
		~BST(){delete root;root=NULL;};

		Node<T>* get_root(){return root;};
		void rebuild(){root=new Node<T>();};

		bool addLeftNode(Node<T>* current, T input);
		bool addRightNode(Node<T>* current, T input);
		void removeNode(Node<T>* current){delete current;current=NULL;}; //USE WITH CAUTION

		//bool buildStandardTree(); // to be implemented someday
		//bool buildDataDependantTree(Node<T>* current, T data); //this meets specific non-standard requirements
};

template<class T>
bool BST<T>::addLeftNode(Node<T>* current, T input){
	if(current->get_lchild()!=NULL){
		return false;		//error reporting: bool dictates if successful, this case was not successful
	}
	else{
		current->set_lchild(input);
		return true;
	}
}

template<class T>
bool BST<T>::addRightNode(Node<T>* current, T input){
	if(current->get_rchild()!=NULL){
		return false;		//error reporting: bool dictates if successful, this case was not successful
	}
	else{
		current->set_rchild(input);
		return true;
	}
}
