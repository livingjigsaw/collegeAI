#include<iostream>
#include<vector>
#include<cassert>
#include"bst.h"

using namespace std;

int main(){
	/*
	----------------------------------------------------------------------------------------------------------------
	begin node testing
	----------------------------------------------------------------------------------------------------------------
	*/
	Node<int>*nodeTest = new Node<int>();
	assert(nodeTest->get_dataAdd() != NULL);
	delete nodeTest;
	nodeTest = new Node<int>(3);
	assert(nodeTest->get_data()==3);
	nodeTest->set_data(5);
	assert(nodeTest->get_data()==5);
	assert(nodeTest->get_lchild()==NULL);
	assert(nodeTest->get_rchild()==NULL);
	nodeTest->set_lchild(1);
	assert(nodeTest->get_data()==5);
	assert(nodeTest->get_lchild()!=NULL);
	assert(nodeTest->get_rchild()==NULL);
	assert(nodeTest->get_lchild()->get_data()==1);
	assert(nodeTest->get_lchild()->get_lchild()==NULL);
	assert(nodeTest->get_lchild()->get_rchild()==NULL);
	nodeTest->set_rchild(2);
	assert(nodeTest->get_data()==5);
	assert(nodeTest->get_lchild()!=NULL);
	assert(nodeTest->get_rchild()!=NULL);
	assert(nodeTest->get_lchild()->get_data()==1);
	assert(nodeTest->get_lchild()->get_lchild()==NULL);
	assert(nodeTest->get_lchild()->get_rchild()==NULL);
	assert(nodeTest->get_rchild()->get_data()==2);
	assert(nodeTest->get_rchild()->get_lchild()==NULL);
	assert(nodeTest->get_rchild()->get_rchild()==NULL);
	Node<int>* rchild = nodeTest->get_rchild();
	assert(rchild->get_data()==2);
	assert(rchild->get_lchild()==NULL);
	assert(rchild->get_rchild()==NULL);
	delete nodeTest;
	assert(rchild->get_data()!=2); //this should prove if children get deleted correctly
	/*
	----------------------------------------------------------------------------------------------------------------
	end node testing
	----------------------------------------------------------------------------------------------------------------
	*/

	/*
	----------------------------------------------------------------------------------------------------------------
	begin tree testing
	----------------------------------------------------------------------------------------------------------------
	*/
	BST<int>* treeTest= new BST<int>();
	assert(treeTest->get_root()!=NULL);
	assert(treeTest->get_root()->get_dataAdd()!=NULL);
	assert(treeTest->get_root()->get_lchild()==NULL);
	assert(treeTest->get_root()->get_rchild()==NULL);
	delete treeTest;
	treeTest = new BST<int>(1);
	assert(treeTest->get_root()!=NULL);
	assert(treeTest->get_root()->get_dataAdd()!=NULL);
	assert(treeTest->get_root()->get_data()==1);
	assert(treeTest->get_root()->get_lchild()==NULL);
	assert(treeTest->get_root()->get_rchild()==NULL);
	Node<int>* tracker = treeTest->get_root();
	assert(tracker->get_dataAdd()!=NULL);
	assert(tracker->get_data()==1);
	assert(tracker->get_lchild()==NULL);
	assert(tracker->get_rchild()==NULL);
	assert(treeTest->addLeftNode(tracker, 2)); //this also sets lchild
	assert(tracker->get_data()==1);
	assert(tracker->get_lchild()!=NULL);
	assert(tracker->get_rchild()==NULL);
	assert(tracker->get_lchild()->get_data()==2);
	assert(tracker->get_lchild()->get_lchild()==NULL);
	assert(tracker->get_lchild()->get_rchild()==NULL);
	assert(!treeTest->addLeftNode(tracker, 3)); //this should not affect lchild
	assert(tracker->get_data()==1);
	assert(tracker->get_lchild()!=NULL);
	assert(tracker->get_rchild()==NULL);
	assert(tracker->get_lchild()->get_data()==2);
	assert(tracker->get_lchild()->get_lchild()==NULL);
	assert(tracker->get_lchild()->get_rchild()==NULL);
	assert(tracker->get_rchild()==NULL);
	assert(treeTest->addRightNode(tracker, 3)); //this also sets rchild
	assert(tracker->get_data()==1);
	assert(tracker->get_lchild()!=NULL);
	assert(tracker->get_rchild()!=NULL);
	assert(tracker->get_lchild()->get_data()==2);
	assert(tracker->get_lchild()->get_lchild()==NULL);
	assert(tracker->get_lchild()->get_rchild()==NULL);
	assert(tracker->get_rchild()->get_data()==3);
	assert(tracker->get_rchild()->get_lchild()==NULL);
	assert(tracker->get_rchild()->get_rchild()==NULL);
	assert(!treeTest->addLeftNode(tracker, 4)); //this should not affect lchild
	assert(tracker->get_data()==1);
	assert(tracker->get_lchild()!=NULL);
	assert(tracker->get_rchild()!=NULL);
	assert(tracker->get_lchild()->get_data()==2);
	assert(tracker->get_lchild()->get_lchild()==NULL);
	assert(tracker->get_lchild()->get_rchild()==NULL);
	assert(tracker->get_rchild()->get_data()==3);
	assert(tracker->get_rchild()->get_lchild()==NULL);
	assert(tracker->get_rchild()->get_rchild()==NULL);
	/*
	----------------------------------------------------------------------------------------------------------------
	end tree testing
	----------------------------------------------------------------------------------------------------------------
	*/
	cout << "test complete. Consider bst.h functional!\n";
}