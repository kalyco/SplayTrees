
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

    /// Splay operation
    void splay(BSTNode<KeyT,ValueT>* aNode)
    {
      // Your implementation goes here
    } 
    
};


#endif
