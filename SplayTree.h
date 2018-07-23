
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
    /// Find a node in the BST containing the given key
    BSTNode<KeyT,ValueT>* find(KeyT aKey)
    {
        BSTNode<KeyT,ValueT>* node = BST<KeyT,ValueT>::find(aKey);
        if (node != nullptr) {
            // Splay the node that is found or the node where the search ended
            splay(node);
        }
        return node;
    }

    /// Find a node in the BST containing the given key.
    /// If exact match not found, return nullptr but also set the approxMatchNode
    BSTNode<KeyT,ValueT>* findApprox(KeyT aKey, 
                                     BSTNode<KeyT,ValueT>*& approxMatchNode_lb,
                                     BSTNode<KeyT,ValueT>*& approxMatchNode_ub)
    {
        BSTNode<KeyT,ValueT>* node = BST<KeyT,ValueT>::findApprox(aKey, 
                                                                  approxMatchNode_lb,
                                                                  approxMatchNode_ub);
        if (node != nullptr) {
            // Splay the node that is found or the node where the search ended
            splay(node);
        } else {
	  if (approxMatchNode_lb)
            splay(approxMatchNode_lb);
	  if (approxMatchNode_ub)
	    splay(approxMatchNode_ub);
        }
        return node;
    }
  
    /// Find min
    BSTNode<KeyT,ValueT>* getMin() 
    { 
        BSTNode<KeyT,ValueT>* minBSTNode = BST<KeyT,ValueT>::getMin(); 
        if (minBSTNode != nullptr) {
            // Splay the min node
            splay(minBSTNode);
        }
        return minBSTNode;
    }

    /// Find max
    BSTNode<KeyT,ValueT>* getMax() 
    { 
        BSTNode<KeyT,ValueT>* maxBSTNode = BST<KeyT,ValueT>::getMax(); 
        if (maxBSTNode != nullptr) {
            // Splay the max node
            splay(maxBSTNode);
        }
        return maxBSTNode;
    }

    /// Remvoe
    void remove(KeyT aKey) 
    { 
        BSTNode<KeyT,ValueT>* parentNode = BST<KeyT,ValueT>::remove(aKey); 
	if (parentNode != nullptr) {
            // Splay the parent node of the removed node
            splay(parentNode);
        }
    }


  public:

    // 1. if aNode < root (zig), get left child or vice versa
    // 2. set the parent of old root as aNode
    // 3. if aNode < root (zig) set Left child as previous
    BSTNode<KeyT,ValueT>* setRotationValues(bool zig, BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent) {
       BSTNode<KeyT,ValueT>* aChild = zig ? aNode->right() : aNode->left();
       zig ? aNode->setRight(aParent) : aNode->setLeft(aParent);
       aParent->setParent(aNode);
       zig ? aParent->setLeft(aChild) : aParent->setRight(aChild); 
       return aChild;
    }

    void updateRoot(BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent, BSTNode<KeyT,ValueT>* aChild) {
        if (aChild != NULL) aChild->setParent(aParent);
        if (this->root() == aParent) {
            this->setRoot(aNode); // Set the new root
            aNode->setParent(NULL);
        }
    }

    // Zig rotation -- left to right. Use-case: aNode is a left child of root
    void zig(BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent) {
        BSTNode<KeyT,ValueT>* rightChild = setRotationValues(true, aNode, aParent);
        updateRoot(aNode, aParent, rightChild);
    }
    
    // Zag rotation -- right to left. Use-case: aNode is a right child of root
    void zag(BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent) {
        BSTNode<KeyT,ValueT>* leftChild = setRotationValues(false, aNode, aParent);
        updateRoot(aNode, aParent, leftChild);
    }

    void doZigOrZag(BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent) {
      aParent->left() == aNode ? zig(aNode, aParent) : zag(aNode, aParent);
    }

    Orientation getOrientation(BSTNode<KeyT,ValueT>* gParent, BSTNode<KeyT,ValueT>* ggParent) {
      if (ggParent != NULL) { // if grandparent is not root
        return (ggParent->left() == gParent ? LEFT : RIGHT); 
      }
      return NONE;
    }

  // Double rotation -- zigzig/zagzag/zagzig/zigzag. Use-case: aNode is a grandchild
  void doubleZigOrZag(BSTNode<KeyT,ValueT>* aNode, BSTNode<KeyT,ValueT>* aParent, BSTNode<KeyT,ValueT>* aGrandParent) {
    BSTNode<KeyT,ValueT>* ggParent = aGrandParent->parent(); // check parent of grandparent
    Orientation orient = getOrientation(aGrandParent, ggParent);
    // TODO: Check the state of these because they could change after the rotations.
    doZigOrZag(aNode, aParent); // Rotate around the parent first
    doZigOrZag(aNode, aGrandParent); // Rotate around the grandparent
    if (orient != NONE) {
        orient == LEFT ? ggParent->setLeft(aNode) : ggParent->setRight(aNode);
        aNode->setParent(ggParent);
    }
  }

    // Splay operation
    void splay(BSTNode<KeyT,ValueT>* aNode) {
    BSTNode<KeyT,ValueT>* parent = aNode->parent();
    if (parent == NULL) return; // already root
    BSTNode<KeyT,ValueT>* grandParent = parent->parent();
    while (parent != NULL && grandParent != NULL) {
      doubleZigOrZag(aNode, parent, grandParent);
      // Evaluate new parent and grandparent after rotations
      parent = aNode->parent();
      if (parent == NULL) break;
      grandParent = parent->parent();
      if (grandParent == NULL) break;
    }
    // Zig operation is done only as a last pass
    // after Zigzig and/or Zigzag operations
    if (grandParent == NULL) doZigOrZag(aNode, parent); // Parent is the root -- Zig operation
  }
};

#endif
