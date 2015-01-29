using namespace std;
//tree is built here to allow non-standard tree creation, as our parameters are not standard
//depth dictates which item from vector to insert into next layer
// always call from main with depth=0

bool solveKnap(BST<set<knapItem*> >* outTree, vector<knapItem*>* inList, Node<set<knapItem*> >* current, treeData inData, Answer& currentSoln, int depth){	
	currentSoln.nodeCount++;
	bool left=true, right=true;
	if(depth==0){
		current = outTree->get_root();
	}
	if(depth==inList->size()){	//no more data to create
		left=false;right=false;
	}
	else{
		if(inData.optims[0] && (inData.currentPotential-((*inList)[depth]->value)) < inData.lowerBound ){
			//do not create left child
			left=false;
		}
		set<knapItem*>::iterator it;
		int costSum=0, valSum=0;
		for(it=current->get_dataAdd()->begin();it!=current->get_dataAdd()->end();it++){
			costSum+=(*it)->cost;
			//valSum+=(*it)->value;
		}
		if(inData.optims[1] && costSum+((*inList)[depth]->cost) > inData.costLimit){
			//do not create right child
			right=false;
		}
	}
	if(!left && !right){	//in leaf node, could be valid soln
		set<knapItem*>::iterator it;
		int costSum=0, valSum=0;
		for(it=current->get_dataAdd()->begin();it!=current->get_dataAdd()->end();it++){
			costSum+=(*it)->cost;
			valSum+=(*it)->value;
		}
		if(valSum > currentSoln.totalValue && costSum <= inData.costLimit){ //i cannot assume i am under the totalcost since i may not be using optimizations
			currentSoln.itemNames.clear();
			currentSoln.totalCost=costSum;
			currentSoln.totalValue=valSum;
			for(it=current->get_dataAdd()->begin();it!=current->get_dataAdd()->end();it++){
				currentSoln.itemNames.insert( (*it)->name );
			}
		}
	}
	else{
		inData.currentPotential-=(*inList)[depth]->value;
		if(left){
			bool leftSuccess=outTree->addLeftNode(current, current->get_data() );
			if(!leftSuccess){
				return false;
			}
			solveKnap(outTree, inList, current->get_lchild(), inData, currentSoln, depth+1);
		}
		if(right){
			inData.currentPotential+=(*inList)[depth]->value;
			set<knapItem*>* tempSet = new set<knapItem*>;
			*tempSet = current->get_data();
			tempSet->insert( (*inList)[depth] );
			bool rightSuccess=outTree->addRightNode(current, *tempSet);
			delete tempSet;
			if(!rightSuccess){
				return false;
			}
			solveKnap(outTree, inList, current->get_rchild(), inData, currentSoln, depth+1);
		}

	}
	outTree->removeNode(current);
	return true;
}