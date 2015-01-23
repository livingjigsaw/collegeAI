using namespace std;
//tree is built here to allow non-standard tree creation, as our parameters are not standard
//depth dictates which item from vector to insert into next layer
// always call from main with depth=0
bool buildTree(BST<set<knapItem*> >* outTree, vector<knapItem*>* inList, Node<set<knapItem*> >* current, int depth){	
	// need to find answer here as well, so that the tree need not be traversed again
	if(depth>=inList->size()){
		return true; 	//base case
	}
	else{
		//if root, current needs to be set
		if(depth==0){
			current = outTree->get_root();
		}
		//use current set for left node. exact same set, why make new ones?
		//if root is current, it should already have a new set from constructor
		depth++;
		bool leftSuccess=outTree->addLeftNode(current, current->get_data() );
		if(!leftSuccess){
			return false;
		}
		buildTree(outTree, inList, current->get_lchild(), depth);
		//copy current set into new set, then add item into that new set based on depth for right node
		set<knapItem*>* tempSet = new set<knapItem*>;
		*tempSet = current->get_data();
		tempSet->insert( (*inList)[depth-1] );
		bool rightSuccess = outTree->addRightNode(current, *tempSet);
		if(!rightSuccess){
			return false;
		}
		buildTree(outTree, inList, current->get_rchild(), depth);

	}
}

//will soon be done as tree is built
void getAnswer(BST<set<knapItem*> >* outTree,  Node<set<knapItem*> >* current, int costLimit, Answer& currentSoln){ //recursive, call with current as root
	if(current->get_lchild()==NULL && current->get_rchild()==NULL){ // i'm in a leaf node

		set<knapItem*>::iterator it;
		int costSum=0, valSum=0;
		for(it=current->get_dataAdd()->begin();it!=current->get_dataAdd()->end();it++){
			costSum+=(*it)->cost;
			valSum+=(*it)->value;
		}
		if(costSum <= costLimit && valSum > currentSoln.totalValue){
			currentSoln.itemNames.clear();
			currentSoln.totalCost=costSum;
			currentSoln.totalValue=valSum;
			for(it=current->get_dataAdd()->begin();it!=current->get_dataAdd()->end();it++){
				currentSoln.itemNames.insert( (*it)->name );
			}
		}
	}
	else{
		getAnswer(outTree, current->get_lchild(), costLimit, currentSoln);
		getAnswer(outTree, current->get_rchild(), costLimit, currentSoln);
	}
}

