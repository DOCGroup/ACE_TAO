// $Id$

// RB_Tree.cpp

#if !defined (ACE_RB_TREE_C)
#define ACE_RB_TREE_C

#include "ace/RB_Tree.h"

#if !defined (__ACE_INLINE__)
#include "ace/RB_Tree.i"
#endif /* __ACE_INLINE__ */

/////////////////////////////////////////
// template class RB_Tree_Node<KEY, T> //
/////////////////////////////////////////

template <class KEY, class T>
RB_Tree_Node<KEY, T>::RB_Tree_Node (const KEY &k, const T &t)
  : k_ (k)
  , t_ (t)
  , color_ (RED)
  , parent_ (0)
  , left_ (0)
  , right_ (0)
{
}
  // constructor

template <class KEY, class T>
RB_Tree_Node<KEY, T>::~RB_Tree_Node ()
{
  // delete left sub-tree
  delete left_;  

  // delete right sub_tree
  delete right_;
}
  // destructor



////////////////////////////////////
// template class RB_Tree<KEY, T> //
////////////////////////////////////

template <class KEY, class T>
RB_Tree<KEY, T>::RB_Tree ()
  : root_ (0)
{
}
  // constructor

template <class KEY, class T>
RB_Tree<KEY, T>::RB_Tree (const RB_Tree<KEY, T> &rbt)
  : root_ (0)  
{
  // make a deep copy of the passed tree
  RB_Tree_Iterator<KEY, T> iter(rbt);
  for (iter.first (); iter.is_done () == 0; iter.next ())
  { 
    insert (*(iter.key ()), *(iter.item ()));
  }
}
  // copy constructor

template <class KEY, class T>
RB_Tree<KEY, T>::~RB_Tree ()
{
  // clear away all nodes in the tree
  clear ();
}
  // destructor

template <class KEY, class T> void
RB_Tree<KEY, T>::operator = (const RB_Tree<KEY, T> &rbt)
{
  // clear out the existing tree
  clear ();

  // make a deep copy of the passed tree
  RB_Tree_Iterator<KEY, T> iter(rbt);
  for (iter.first (); iter.is_done () == 0; iter.next ())
  { 
    insert (*(iter.key ()), *(iter.item ()));
  }
}
  // assignment operator

template <class KEY, class T> T* 
RB_Tree<KEY, T>::find (const KEY &k)
{
  // find the closest matching node, if there is one
  RB_Tree_Node<KEY, T> *current = find_node (k);

  if (current)
  {
    // if a nearest matching node was returned
    if ((current->key () < k) || (k < current->key ()))
    {
      // if the keys differ, there is no match: return 0
      return 0;
    }
    else
    {
      // else, the keys match: return a pointer to the node's item
      return &(current->item ());
    }
  }
  else
  {
    // else, the tree is empty: return 0
    return 0;
  }
}
  // Returns a pointer to the item corresponding to the
  // given key, or 0 if it cannot find the key in the tree.

template <class KEY, class T>  T* 
RB_Tree<KEY, T>::insert (const KEY &k, const T &t)
{
  // find the closest matching node, if there is one
  RB_Tree_Node<KEY, T> *current = find_node (k);
  if (current)
  {
    if (current->key () < k)
    {
      // if a nearest matching node has a key less than the insertion key
      if (current->right ())
      {
        // if there is already a right subtree, complain
        ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
                           ASYS_TEXT ("\nright subtree already present in "
                                      "RB_Tree<KEY, T>::insert\n")), 0);
      }
      else
      {
        // else, the right subtree is empty: insert new node there
        current->right (new RB_Tree_Node<KEY, T> (k, t));
        if (current->right ())
        {
          // if the node was successfully inserted, set its parent, rebalance the
          // tree, color the root black, and return a pointer to the inserted item
          T *item = &(current->right ()->item ());
          current->right ()->parent (current);
          RB_rebalance (current->right ());
          root_->color (BLACK);
          return item;
        }
        else
        {
          // else, memory allocation failed
        ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
                           ASYS_TEXT ("\nmemory allocation to current->right_ failed "
                                      "in RB_Tree<KEY, T>::insert\n")), 0);
        }
      }
    }
    else if (k < current->key ())
    {
      // if a nearest matching node has a key greater than the insertion key
      if (current->left ())
      {
        // if there is already a left subtree, complain
        ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
                           ASYS_TEXT ("\nleft subtree already present in "
                                      "RB_Tree<KEY, T>::insert\n")), 0);
      }
      else
      {
        // else, the right subtree is empty: insert new node there
        current->left (new RB_Tree_Node<KEY, T> (k, t));
        if (current->left ())
        {
          // if the node was successfully inserted, set its parent, rebalance the
          // tree, color the root black, and return a pointer to the inserted item
          T *item = &(current->left ()->item ());
          current->left ()->parent (current);
          RB_rebalance (current->left ());
          root_->color (BLACK);
          return item;
        }
        else
        {
          // else, memory allocation failed
          ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
                           ASYS_TEXT ("\nmemory allocation to current->left_ failed in "
                                      "RB_Tree<KEY, T>::insert\n")), 0);
        }
      }
    }
    else
    {
      // else, the keys match: return a pointer to the node's item
      return &(current->item ());
    }
  }
  else
  {
    // else, the tree is empty: insert at the root and color the root black
    root_ = new RB_Tree_Node<KEY, T> (k, t);
    if (root_)
    {
      root_->color (BLACK);
      return &(root_->item ());
    }
    else
    {
      ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
                         ASYS_TEXT ("\nmemory allocation to root_ failed in "
                                    "RB_Tree<KEY, T>::insert\n")), 0);
    }
  }
}
  // Inserts a *copy* of the key and the item into the tree:
  // both the key type KEY and the item type T must have well
  // defined semantics for copy construction and < comparison.
  // This method returns a pointer to the inserted item copy,
  // or 0 if an error occurred.  NOTE: if an identical key
  // already exists in the tree, no new item is created, and
  // the returned pointer addresses the existing item
  // associated with the existing key.

template <class KEY, class T>  int 
RB_Tree<KEY, T>::remove (const KEY &k)
{
  // find a matching node, if there is one
  RB_Tree_Node<KEY, T> *z = find_node (k);

  if ((z) && (! (z->key () < k)) && (! (k < z->key ())))
  {
    // there is a matching node: remove and destroy it
    RB_Tree_Node<KEY, T> *y
    if ((z->left ()) && (z->right ()))
    {
      y = RB_tree_successor (z);
    }    
    else
    {
      y = z; 
    }    
    if (y->left ())
    {
      x = y->left ();
    }
    else
    {
      x = y->right ();
    }
    x->parent (y->parent ());
    if (y->parent ())
    {
      if (y == y->parent ()->left ())
      {
        y->parent ()->left (x);
      }
      else
      {
        y->parent ()->right (x);
      }
    }
    else
    {
      root_ = x;
    }
    if (y != z)
    {
      // copy the elements of y into z
      z->key () = y->key ();
      z->item () = y->item ();
    }
    if (y->color () == RB_Tree_Node<KEY, T>::BLACK)
    {
      RB_delete_fixup (x);
    }
    y->parent (0);
    y->right (0);
    y->left (0);
    delete y;
    return 1;
  }
  else
  {
    // else, no matching node was found: return 0
    return 0;
  }
}
  // removes the item associated with the given key from the 
  // tree and destroys it.  Returns 1 if it found the item
  // and successfully destroyed it, 0 if it did not find the
  // item, or -1 if an error occurred.


template <class KEY, class T>  void 
RB_Tree<KEY, T>::RB_rotate_right (RB_Tree_Node<KEY, T> * x)
{
  if (! x) 
  {
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
                       ASYS_TEXT ("\nerror: x is a null pointer in "
                                  "RB_Tree<KEY, T>::RB_rotate_right\n")));
  }
  else if (! (x->left()))
  {
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
                ASYS_TEXT ("\nerror: x->left () is a null pointer in "
                           "RB_Tree<KEY, T>::RB_rotate_right\n")));
  }
  else
  {
    RB_Tree_Node<KEY, T> * y;
    y = x->left ();
    x->left (y->right ());
    if (y->right ())
    {
      y->right ()->parent (x);
    }
    y->parent (x->parent ());
    if (x->parent ())
    {
      if (x == x->parent ()->right ())
      {
        x->parent ()->right (y);
      }
      else
      {
        x->parent ()->left (y);
      }
    }
    else
    {
      root_ = y;
    }
    y->right (x);
    x->parent (y);
  }
}
  // method for right rotation of the tree about a given node


template <class KEY, class T> void 
RB_Tree<KEY, T>::RB_rotate_left (RB_Tree_Node<KEY, T> * x)
{
  if (! x) 
  {
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
                ASYS_TEXT ("\nerror: x is a null pointer in "
                           "RB_Tree<KEY, T>::RB_rotate_left\n")));
  }
  else if (! (x->right()))
  {
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
                ASYS_TEXT ("\nerror: x->right () is a null pointer "
                           "in RB_Tree<KEY, T>::RB_rotate_left\n")));
  }
  else
  {
    RB_Tree_Node<KEY, T> * y;
    y = x->right ();
    x->right (y->left ());
    if (y->left ())
    {
      y->left ()->parent (x);
    }
    y->parent (x->parent ());
    if (x->parent ())
    {
      if (x == x->parent ()->left ())
      {
        x->parent ()->left (y);
      }
      else
      {
        x->parent ()->right (y);
      }
    }
    else
    {
      root_ = y;
    }
    y->left (x);
    x->parent (y);
  }
}
  // method for left rotation of the tree about a given node

template <class KEY, class T>  void 
RB_Tree<KEY, T>::RB_delete_fixup (RB_Tree_Node<KEY, T> * x)
{
  while ((x) && (x->parent ()) && (x->color () == RB_Tree_Node<KEY, T>::BLACK))
  {
    if (x == x->parent ()->left ())
    {
      RB_Tree_Node<KEY, T> *w = x->parent ()->right ();
      if (w->color () == RB_Tree_Node<KEY, T>::RED)
      {
        w->color (RB_Tree_Node<KEY, T>::BLACK);
        x->parent ()->color (RB_Tree_Node<KEY, T>::RED);
        RB_rotate_left (x->parent ());
        w = x->parent ()->right ();
      }
      if ((w->left ()->color () == RB_Tree_Node<KEY, T>::BLACK) &&
          (w->right ()->color () == RB_Tree_Node<KEY, T>::BLACK))
      {
        w->color (RB_Tree_Node<KEY, T>::RED);
        x = x->parent ();
      }
      else
      {
        if (w->right ()->color () == RB_Tree_Node<KEY, T>::BLACK)
        {
          w->left ()->color (RB_Tree_Node<KEY, T>::BLACK);
          w->color (RB_Tree_Node<KEY, T>::RED);
          RB_rotate_right (w);
          w = x->parent->right ();
        }
        w->color (x->parent->color ());
        x->parent->color (RB_Tree_Node<KEY, T>::BLACK);
        w->right->color (RB_Tree_Node<KEY, T>::BLACK);
        RB_rotate_left (x->parent ());
        x = root_;
      }
    }
    else
    {
      RB_Tree_Node<KEY, T> *w = x->parent ()->left ();
      if (w->color () == RB_Tree_Node<KEY, T>::RED)
      {
        w->color (RB_Tree_Node<KEY, T>::BLACK);
        x->parent ()->color (RB_Tree_Node<KEY, T>::RED);
        RB_rotate_right (x->parent ());
        w = x->parent ()->left ();
      }
      if ((w->left ()->color () == RB_Tree_Node<KEY, T>::BLACK) &&
          (w->right ()->color () == RB_Tree_Node<KEY, T>::BLACK))
      {
        w->color (RB_Tree_Node<KEY, T>::RED);
        x = x->parent ();
      }
      else
      {
        if (w->left ()->color () == RB_Tree_Node<KEY, T>::BLACK)
        {
          w->right ()->color (RB_Tree_Node<KEY, T>::BLACK);
          w->color (RB_Tree_Node<KEY, T>::RED);
          RB_rotate_left (w);
          w = x->parent->left ();
        }
        w->color (x->parent->color ());
        x->parent->color (RB_Tree_Node<KEY, T>::BLACK);
        w->left->color (RB_Tree_Node<KEY, T>::BLACK);
        RB_rotate_right (x->parent ());
        x = root_;
      }
    }
  }

  if (x)
  {
    x->color (RB_Tree_Node<KEY, T>::BLACK);
  }
}
  // method for restoring Red-Black properties after deletion

template <class KEY, class T> RB_Tree_Node<KEY, T> * 
RB_Tree<KEY, T>::find_node (const KEY &k)
{
  RB_Tree_Node<KEY, T> *current = root_;

  while (current)
  {
    // while there are more nodes to examine
    if (current->key () < k)
    {
      // if the search key is greater than the current node's key
      if (current->right ())
      {
        // if the right subtree is not empty, search to the right
        current = current->right ();
      }
      else
      {
        // if the right subtree is empty, we're done
        break;
      }
    }
    else if (k < current->key ())
    {
      // else if the search key is less than the current node's key
      if (current->left ())
      {
        // if the left subtree is not empty, search to the left
        current = current->left ();
      }
      else
      {
        // if the left subtree is empty, we're done
        break;
      }
    }    
    else
    {
      // if the keys match, we're done
      break;
    }
  }

  return current;
}
  // returns a pointer to a matching node if there is one,
  // a pointer to the node under which to insert the item
  // if the tree is not empty and there is no such match,
  // or 0 if the tree is empty.

template <class KEY, class T> void 
RB_Tree<KEY, T>::RB_rebalance (RB_Tree_Node<KEY, T> * x)
{
  RB_Tree_Node<KEY, T> *y = 0;

  while ((x) && (x->parent ()) && (x->parent ()->color () == RED))
  {
    if (! x->parent ()->parent ())
    {
      // if we got here, something is drastically wrong!
      ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  
                  ASYS_TEXT ("\nerror: parent's parent is null in "
                             "RB_Tree<KEY, T>::RB_rebalance\n")));
      return;
    }

    if (x->parent () == x->parent ()->parent ()->left ())
    {
      y = x->parent ()->parent ()->right ();
      if (y && (y->color () == RED))
      {
        // handle case 1 (see CLR book, pp. 269)
        x->parent ()->color (BLACK);
        y->color (BLACK);
        x->parent ()->parent ()->color (RED);
        x = x->parent ()->parent ();
      }
      else 
      {
        if (x == x->parent ()->right ())
        {
          // transform case 2 into case 3 (see CLR book, pp. 269)
          x = x->parent ();
          RB_rotate_left (x);
        }

        // handle case 3 (see CLR book, pp. 269)
        x->parent ()->color (BLACK);
        x->parent ()->parent ()->color (RED);
        RB_rotate_right (x->parent ()->parent ());
      }
    }
    else
    {
      y = x->parent ()->parent ()->left ();
      if (y && (y->color () == RED))
      {
        // handle case 1 (see CLR book, pp. 269)
        x->parent ()->color (BLACK);
        y->color (BLACK);
        x->parent ()->parent ()->color (RED);
        x = x->parent ()->parent ();
      }
      else 
      {
        if (x == x->parent ()->left ())
        {
          // transform case 2 into case 3 (see CLR book, pp. 269)
          x = x->parent ();
          RB_rotate_right (x);
        }

        // handle case 3 (see CLR book, pp. 269)
        x->parent ()->color (BLACK);
        x->parent ()->parent ()->color (RED);
        RB_rotate_left (x->parent ()->parent ());
      }
    }
  }
}
  // rebalance the tree after insertion of a node

template <class KEY, class T> RB_Tree_Node<KEY, T> * 
RB_Tree<KEY, T>::RB_tree_successor (RB_Tree_Node<KEY, T> *x) const
{
  if (x->right ())
  {
    return RB_tree_minimum (x->right ());
  }

  RB_Tree_Node<KEY, T> *y = x->parent ();
  while ((y) && (x == y->right ()))
  {
    x = y;
    y = y->parent ();
  }

  return y;
}
  // method to find the successor node of the given node in the tree

template <class KEY, class T> RB_Tree_Node<KEY, T> * 
RB_Tree<KEY, T>::RB_tree_predecessor (RB_Tree_Node<KEY, T> *x) const
{
  if (x->left ())
  {
    return RB_tree_maximum (x->left ());
  }

  RB_Tree_Node<KEY, T> *y = x->parent ();
  while ((y) && (x == y->left ()))
  {
    x = y;
    y = y->parent ();
  }

  return y;
}
  // method to find the predecessor node of the given node in the tree

template <class KEY, class T> RB_Tree_Node<KEY, T> * 
RB_Tree<KEY, T>::RB_tree_minimum (RB_Tree_Node<KEY, T> *x) const
{
  while ((x) && (x->left ()))
  {
    x = x->left ();
  }

  return x;
}
  // method to find the minimum node of the subtree rooted at the given node


template <class KEY, class T> RB_Tree_Node<KEY, T> * 
RB_Tree<KEY, T>::RB_tree_maximum (RB_Tree_Node<KEY, T> *x) const
{
  while ((x) && (x->right ()))
  {
    x = x->right ();
  }

  return x;
}
  // method to find the maximum node of the subtree rooted at the given node



/////////////////////////////////////////////
// template class RB_Tree_Iterator<KEY, T> //
/////////////////////////////////////////////

template <class KEY, class T>
RB_Tree_Iterator<KEY, T>::RB_Tree_Iterator (const RB_Tree<KEY, T> &tree)
  : tree_ (tree), node_ (0)  
{
  // position the iterator at the first node in the tree
  first ();
}  
  // constructor

template <class KEY, class T>
RB_Tree_Iterator<KEY, T>::~RB_Tree_Iterator ()
{
}
  // destructor


#endif /* !defined (ACE_RB_TREE_C) */

