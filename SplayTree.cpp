// SplayTree.cpp

#include <utility> 
#include <iostream>
#include "bst.h"
#include "SplayTree.h"

using namespace std;

template<typename KeyT, typename ValueT>
BSTNode<KeyT,ValueT>* SplayTree<KeyT, ValueT>::setRotationValues(bool zig, BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent) {
	BSTNode<KeyT,ValueT>* aChild = zig ? aNode->right() : aNode->left();
	zig ? aNode->setRight(aParent) : aNode->setLeft(aParent);
	aParent->setParent(aNode);
	zig ? aParent->setLeft(aChild) : aParent->setRight(aChild);
	return aChild;
}
