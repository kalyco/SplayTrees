
#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

#include <iostream>
#include "Array.h"
#include "bst.h"
#include <cassert>

using namespace std;

template<typename KeyT, typename ValueT>
class SplayTree : public BST<KeyT,ValueT> {
  public:  
    enum Orientation {
        NONE, LEFT, RIGHT
    };

    /// Find a node in the BST containing the given key.
    /// If exact match not found, return nullptr but also set the approxMatchNode
    BSTNode<KeyT,ValueT>* findApprox(KeyT aKey, 
																		 BSTNode<KeyT,ValueT>*& approxMatchNode_lb,
																		 BSTNode<KeyT,ValueT>*& approxMatchNode_ub) {
      BSTNode<KeyT,ValueT>* node = BST<KeyT,ValueT>::findApprox(aKey, approxMatchNode_lb, approxMatchNode_ub);
      if (node != nullptr) {
				splay(node); // Splay the node that is found or the node where the search ended
  		} else {
	  		if (approxMatchNode_lb) splay(approxMatchNode_lb);
	  		if (approxMatchNode_ub) splay(approxMatchNode_ub);
    	}
      return node;
    }
  
    /// Find min
    BSTNode<KeyT,ValueT>* getMin() { 
      BSTNode<KeyT,ValueT>* minBSTNode = BST<KeyT,ValueT>::getMin(); 
			if (minBSTNode != nullptr) splay(minBSTNode);
			return minBSTNode;
    }

    /// Find max
    BSTNode<KeyT,ValueT>* getMax() { 
      BSTNode<KeyT,ValueT>* maxBSTNode = BST<KeyT,ValueT>::getMax(); 
			if (maxBSTNode != nullptr) splay(maxBSTNode);
			return maxBSTNode;
    }

		/////////////////////////////////////
		////     Modifier Operations    ////
		///////////////////////////////////

		BSTNode<KeyT,ValueT>* find(KeyT aKey) {
			BSTNode<KeyT,ValueT>* node = BST<KeyT,ValueT>::find(aKey);
			if (node) splay(node);
			return node;
		}

		BSTNode<KeyT,ValueT>* insert(KeyT aKey) {
			BSTNode<KeyT,ValueT>* node = BST<KeyT,ValueT>::insert(aKey);
			if (node) splay(node);
			return node;
		}

		void remove(KeyT aKey) { 
			BSTNode<KeyT,ValueT>* parentNode = BST<KeyT,ValueT>::remove(aKey); 
			if (parentNode) splay(parentNode);
		}

		/////////////////////////////////////
		////      Splay Operations      ////
		///////////////////////////////////

		BSTNode<KeyT,ValueT>* setRotationValues(bool zig, BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent) {
			BSTNode<KeyT,ValueT>* aChild = zig ? aNode->right() : aNode->left();
			zig ? aNode->setRight(aParent) : aNode->setLeft(aParent);
			aParent->setParent(aNode);
			zig ? aParent->setLeft(aChild) : aParent->setRight(aChild);
			return aChild;
		}

		void updateRoot(BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent, BSTNode<KeyT,ValueT>* aChild) {
			if (aChild != nullptr) aChild->setParent(aParent);
			if (this->root() == aParent) {
					this->setRoot(aNode); // Set the new root
					aNode->setParent(nullptr);
			}
		}

		// // Zig rotation -- left to right. Use-case: aNode is a left child of root
		void zig(BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent) {
			BSTNode<KeyT,ValueT>* rightChild = setRotationValues(true, aNode, aParent);
			updateRoot(aNode, aParent, rightChild);
		}
			
		// // Zag rotation -- right to left. Use-case: aNode is a right child of root
		void zag(BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent) {
			BSTNode<KeyT,ValueT>* leftChild = setRotationValues(false, aNode, aParent);
			updateRoot(aNode, aParent, leftChild);
		}

		// Double rotation -- zigzig/zagzag/zagzig/zigzag. Use-case: aNode is a grandchild
		void doubleZigOrZag(BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* parent, BSTNode<KeyT,ValueT>* grandParent) {
			if (!parent || !grandParent) return;
			pair<int,int> directions = checkDouble(aNode); // note the current state
			BSTNode<KeyT,ValueT>* ggParent = grandParent->parent(); // great grandparent
			Orientation orient = getOrientation(grandParent, ggParent);
			directions.first == 1 ? zig(aNode, parent) : zag(aNode, parent); // Rotate around parent
			directions.second == 1 ? zig(aNode, grandParent) : zag(aNode, grandParent); // Rotate around grandparent
			if (orient != NONE) {
				orient == LEFT ? ggParent->setLeft(aNode) : ggParent->setRight(aNode);
				aNode->setParent(ggParent); // aNode is now root. Set ggParent as parent.
			}
		}

		bool isZig(BSTNode<KeyT,ValueT>* aNode) { return aNode->parent()->left() == aNode; }

		void doZigOrZag(BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent) {
			isZig(aNode) ? zig(aNode, aParent) : zag(aNode, aParent);
		}

		pair<int,int> checkDouble(BSTNode<KeyT,ValueT>* aNode) {
			pair<int,int> directions;
			directions.first = isZig(aNode); // check child node orientation
			directions.second = isZig(aNode->parent()); // check parent node orientation
			return directions;
		}

		Orientation getOrientation(BSTNode<KeyT,ValueT>* grandParent, BSTNode<KeyT,ValueT>* ggParent) {
			if (ggParent) return (ggParent->left() == grandParent ? LEFT : RIGHT); 
			return NONE;
		}

		void splay(BSTNode<KeyT,ValueT>* aNode) {
			if (!aNode || !aNode->parent()) return;
			BSTNode<KeyT,ValueT>* parent = aNode->parent();
			BSTNode<KeyT,ValueT>* grandParent = parent->parent();
			while (parent && grandParent) {
				doubleZigOrZag(aNode, parent, grandParent);

				parent = aNode->parent(); // eval new parent and gparent after rotations
				if (!parent) break;
				grandParent = parent->parent();
				if (!grandParent)break;
			}
			if (!grandParent && parent) doZigOrZag(aNode, parent);
		}
};	

#endif
