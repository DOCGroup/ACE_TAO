// $Id$

// RB_Tree.cpp

#ifndef ACE_RB_TREE_C
#define ACE_RB_TREE_C

#include "ace/RB_Tree.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/RB_Tree.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, RB_Tree, "$Id$")

/////////////////////////////////////////////
// template class ACE_RB_Tree_Node<KEY, T> //
/////////////////////////////////////////////


// Constructor.

template <class KEY, class T>
ACE_RB_Tree_Node<KEY, T>::ACE_RB_Tree_Node (const KEY &k, const T &t)
  : k_ (k)
  , t_ (t)
  , color_ (RED)
  , parent_ (0)
  , left_ (0)
  , right_ (0)
{
}


// Destructor.

template <class KEY, class T>
ACE_RB_Tree_Node<KEY, T>::~ACE_RB_Tree_Node ()
{
  // Delete left sub-tree.
  delete left_;

  // Delete right sub_tree.
  delete right_;
}



////////////////////////////////////////
// template class ACE_RB_Tree<KEY, T> //
////////////////////////////////////////

// Constructor.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::ACE_RB_Tree ()
  : root_ (0)
{
}


// Copy constructor.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::ACE_RB_Tree (const ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK> &rbt)
  : root_ (0)
{
  // Make a deep copy of the passed tree.
  ACE_RB_Tree_Iterator<KEY, T, COMPARE_KEYS, ACE_LOCK> iter(rbt);
  for (iter.first (); iter.is_done () == 0; iter.next ())
  {
    insert (*(iter.key ()), *(iter.item ()));
  }
}


// Destructor.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::~ACE_RB_Tree ()
{
  // Clear away all nodes in the tree.
  clear ();
}


// Assignment operator.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK> void
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::operator = (const ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK> &rbt)
{
  // Clear out the existing tree.
  clear ();

  // Make a deep copy of the passed tree.
  ACE_RB_Tree_Iterator<KEY, T, COMPARE_KEYS, ACE_LOCK> iter(rbt);
  for (iter.first (); iter.is_done () == 0; iter.next ())
  {
    insert (*(iter.key ()), *(iter.item ()));
  }
}

// Less than comparison function for keys, default
// functor implementation returns 1 if k1 < k2, 0 otherwise.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK> int
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::lessthan (const KEY &k1, const KEY &k2)
{
      return this->compare_keys_ (k1, k2);
}


// Returns a pointer to the item corresponding to the
// given key, or 0 if it cannot find the key in the tree.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK> T*
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::find (const KEY &k)
{
  // Find the closest matching node, if there is one.
  ACE_RB_Tree_Node<KEY, T> *current = find_node (k);

  if (current)
  {
    // If a nearest matching node was returned.
    if (this->lessthan (current->key (), k)
        || this->lessthan (k, current->key ()))
    {
      // If the keys differ, there is no match: return 0.
      return 0;
    }
    else
    {
      // The keys match: return a pointer to the node's item.
      return &(current->item ());
    }
  }
  else
  {
    // The tree is empty: return 0.
    return 0;
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

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>  T*
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::insert (const KEY &k, const T &t)
{
  // Find the closest matching node, if there is one.
  ACE_RB_Tree_Node<KEY, T> *current = find_node (k);
  if (current)
  {
    if (this->lessthan (current->key (), k))
    {
      // If a nearest matching node has a key less than the insertion key.
      if (current->right ())
      {
        // If there is already a right subtree, complain.
        ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("\nright subtree already present in "
                                      "ACE_RB_Tree<KEY, T>::insert\n")), 0);
      }
      else
      {
        // The right subtree is empty: insert new node there.
        current->right (new ACE_RB_Tree_Node<KEY, T> (k, t));
        if (current->right ())
        {
          // If the node was successfully inserted, set its parent, rebalance
          // the tree, color the root black, and return a pointer to the
          // inserted item.
          T *item = &(current->right ()->item ());
          current->right ()->parent (current);
          RB_rebalance (current->right ());
          root_->color (ACE_RB_Tree_Node_Base::BLACK);
          return item;
        }
        else
        {
          // Memory allocation failed.
        ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("\nmemory allocation to current->right_ failed "
                                      "in ACE_RB_Tree<KEY, T>::insert\n")), 0);
        }
      }
    }
    else if (this->lessthan (k, current->key ()))
    {
      // If a nearest matching node has a key greater than the insertion key.
      if (current->left ())
      {
        // If there is already a left subtree, complain.
        ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("\nleft subtree already present in "
                                      "ACE_RB_Tree<KEY, T>::insert\n")), 0);
      }
      else
      {
        // The right subtree is empty: insert new node there.
        current->left (new ACE_RB_Tree_Node<KEY, T> (k, t));
        if (current->left ())
        {
          // If the node was successfully inserted, set its parent, rebalance
          // the tree, color the root black, and return a pointer to the
          // inserted item.
          T *item = &(current->left ()->item ());
          current->left ()->parent (current);
          RB_rebalance (current->left ());
          root_->color (ACE_RB_Tree_Node_Base::BLACK);
          return item;
        }
        else
        {
          // Memory allocation failed.
          ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),
                           ASYS_TEXT ("\nmemory allocation to current->left_ failed in "
                                      "ACE_RB_Tree<KEY, T>::insert\n")), 0);
        }
      }
    }
    else
    {
      // The keys match: return a pointer to the node's item.
      return &(current->item ());
    }
  }
  else
  {
    // The tree is empty: insert at the root and color the root black.
    root_ = new ACE_RB_Tree_Node<KEY, T> (k, t);
    if (root_)
    {
      root_->color (ACE_RB_Tree_Node_Base::BLACK);
      return &(root_->item ());
    }
    else
    {
      ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),
                         ASYS_TEXT ("\nmemory allocation to root_ failed in "
                                    "ACE_RB_Tree<KEY, T>::insert\n")), 0);
    }
  }
}


// Removes the item associated with the given key from the
// tree and destroys it.  Returns 1 if it found the item
// and successfully destroyed it, 0 if it did not find the
// item, or -1 if an error occurred.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>  int
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::remove (const KEY &k)
{
  // Find a matching node, if there is one.
  ACE_RB_Tree_Node<KEY, T> *x, *z;

  z = find_node (k);

  if ((z) && (! this->lessthan (z->key (), k))
      && (! this->lessthan (k, z->key ())))
  {
    // There is a matching node: remove and destroy it.
    ACE_RB_Tree_Node<KEY, T> *y;
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
    if (x)
        {
          x->parent (y->parent ());
        }
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
      // Copy the elements of y into z.
      z->key () = y->key ();
      z->item () = y->item ();
    }
    // CLR pp. 263 says that nil nodes are implicitly colored BLACK
    if (!y || y->color () == ACE_RB_Tree_Node_Base::BLACK)
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
    // No matching node was found: return 0.
    return 0;
  }
}


// Method for right rotation of the tree about a given node.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>  void
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::RB_rotate_right (ACE_RB_Tree_Node<KEY, T> * x)
{
  if (! x)
  {
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("\nerror: x is a null pointer in "
                                  "ACE_RB_Tree<KEY, T>::RB_rotate_right\n")));
  }
  else if (! (x->left()))
  {
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("\nerror: x->left () is a null pointer in "
                           "ACE_RB_Tree<KEY, T>::RB_rotate_right\n")));
  }
  else
  {
    ACE_RB_Tree_Node<KEY, T> * y;
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


// Method for left rotation of the tree about a given node.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK> void
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::RB_rotate_left (ACE_RB_Tree_Node<KEY, T> * x)
{
  if (! x)
  {
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("\nerror: x is a null pointer in "
                           "ACE_RB_Tree<KEY, T>::RB_rotate_left\n")));
  }
  else if (! (x->right()))
  {
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),
                ASYS_TEXT ("\nerror: x->right () is a null pointer "
                           "in ACE_RB_Tree<KEY, T>::RB_rotate_left\n")));
  }
  else
  {
    ACE_RB_Tree_Node<KEY, T> * y;
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


// Method for restoring Red-Black properties after deletion.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>  void
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::RB_delete_fixup (ACE_RB_Tree_Node<KEY, T> * x)
{
  while (x && 
         x->parent () && 
         x->color () == ACE_RB_Tree_Node_Base::BLACK)
  {
    if (x == x->parent ()->left ())
    {
      ACE_RB_Tree_Node<KEY, T> *w = x->parent ()->right ();
      if (w && w->color () == ACE_RB_Tree_Node_Base::RED)
      {
        w->color (ACE_RB_Tree_Node_Base::BLACK);
        x->parent ()->color (ACE_RB_Tree_Node_Base::RED);
        RB_rotate_left (x->parent ());
        w = x->parent ()->right ();
      }
      // CLR pp. 263 says that nil nodes are implicitly colored BLACK
      if ((w) && 
          (!w->left () || 
           w->left ()->color () == ACE_RB_Tree_Node_Base::BLACK) &&
          (!w->right () ||
           w->right ()->color () == ACE_RB_Tree_Node_Base::BLACK))
      {
        w->color (ACE_RB_Tree_Node_Base::RED);
        x = x->parent ();
      }
      else
      {
        // CLR pp. 263 says that nil nodes are implicitly colored BLACK
        if (w && 
            (!w->right () || 
             w->right ()->color () == ACE_RB_Tree_Node_Base::BLACK))
        {
          w->left ()->color (ACE_RB_Tree_Node_Base::BLACK);
          w->color (ACE_RB_Tree_Node_Base::RED);
          RB_rotate_right (w);
          w = x->parent ()->right ();
        }
        w->color (x->parent ()->color ());
        x->parent ()->color (ACE_RB_Tree_Node_Base::BLACK);
        w->right ()->color (ACE_RB_Tree_Node_Base::BLACK);
        RB_rotate_left (x->parent ());
        x = root_;
      }
    }
    else
    {
      ACE_RB_Tree_Node<KEY, T> *w = x->parent ()->left ();
      if (w && w->color () == ACE_RB_Tree_Node_Base::RED)
      {
        w->color (ACE_RB_Tree_Node_Base::BLACK);
        x->parent ()->color (ACE_RB_Tree_Node_Base::RED);
        RB_rotate_right (x->parent ());
        w = x->parent ()->left ();
      }
      // CLR pp. 263 says that nil nodes are implicitly colored BLACK
      if ((!w->left () || 
           w->left ()->color () == ACE_RB_Tree_Node_Base::BLACK) &&
          (!w->right () ||
           w->right ()->color () == ACE_RB_Tree_Node_Base::BLACK))
      {
        w->color (ACE_RB_Tree_Node_Base::RED);
        x = x->parent ();
      }
      else
      {
        // CLR pp. 263 says that nil nodes are implicitly colored BLACK
        if (!w->left () || 
            w->left ()->color () == ACE_RB_Tree_Node_Base::BLACK)
        {
          w->right ()->color (ACE_RB_Tree_Node_Base::BLACK);
          w->color (ACE_RB_Tree_Node_Base::RED);
          RB_rotate_left (w);
          w = x->parent ()->left ();
        }
        w->color (x->parent ()->color ());
        x->parent ()->color (ACE_RB_Tree_Node_Base::BLACK);
        w->left ()->color (ACE_RB_Tree_Node_Base::BLACK);
        RB_rotate_right (x->parent ());
        x = root_;
      }
    }
  }

  if (x)
  {
    x->color (ACE_RB_Tree_Node_Base::BLACK);
  }
}



// Return a pointer to a matching node if there is one,
// a pointer to the node under which to insert the item
// if the tree is not empty and there is no such match,
// or 0 if the tree is empty.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK> ACE_RB_Tree_Node<KEY, T> *
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::find_node (const KEY &k)
{
  ACE_RB_Tree_Node<KEY, T> *current = root_;

  while (current)
  {
    // While there are more nodes to examine.
    if (this->lessthan (current->key (), k))
    {
      // If the search key is greater than the current node's key.
      if (current->right ())
      {
        // If the right subtree is not empty, search to the right.
        current = current->right ();
      }
      else
      {
        // If the right subtree is empty, we're done.
        break;
      }
    }
    else if (this->lessthan (k, current->key ()))
    {
      // Else if the search key is less than the current node's key.
      if (current->left ())
      {
        // If the left subtree is not empty, search to the left.
        current = current->left ();
      }
      else
      {
        // If the left subtree is empty, we're done.
        break;
      }
    }
    else
    {
      // If the keys match, we're done.
      break;
    }
  }

  return current;
}


// Rebalance the tree after insertion of a node.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK> void
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::RB_rebalance (ACE_RB_Tree_Node<KEY, T> * x)
{
  ACE_RB_Tree_Node<KEY, T> *y = 0;

  while (x && 
         x->parent () &&
         x->parent ()->color () == ACE_RB_Tree_Node_Base::RED)
  {
    if (! x->parent ()->parent ())
    {
      // If we got here, something is drastically wrong!
      ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),
                  ASYS_TEXT ("\nerror: parent's parent is null in "
                             "ACE_RB_Tree<KEY, T>::RB_rebalance\n")));
      return;
    }

    if (x->parent () == x->parent ()->parent ()->left ())
    {
      y = x->parent ()->parent ()->right ();
      if (y && y->color () == ACE_RB_Tree_Node_Base::RED)
      {
        // Handle case 1 (see CLR book, pp. 269).
        x->parent ()->color (ACE_RB_Tree_Node_Base::BLACK);
        y->color (ACE_RB_Tree_Node_Base::BLACK);
        x->parent ()->parent ()->color (ACE_RB_Tree_Node_Base::RED);
        x = x->parent ()->parent ();
      }
      else
      {
        if (x == x->parent ()->right ())
        {
          // Transform case 2 into case 3 (see CLR book, pp. 269).
          x = x->parent ();
          RB_rotate_left (x);
        }

        // Handle case 3 (see CLR book, pp. 269).
        x->parent ()->color (ACE_RB_Tree_Node_Base::BLACK);
        x->parent ()->parent ()->color (ACE_RB_Tree_Node_Base::RED);
        RB_rotate_right (x->parent ()->parent ());
      }
    }
    else
    {
      y = x->parent ()->parent ()->left ();
      if (y && y->color () == ACE_RB_Tree_Node_Base::RED)
      {
        // Handle case 1 (see CLR book, pp. 269).
        x->parent ()->color (ACE_RB_Tree_Node_Base::BLACK);
        y->color (ACE_RB_Tree_Node_Base::BLACK);
        x->parent ()->parent ()->color (ACE_RB_Tree_Node_Base::RED);
        x = x->parent ()->parent ();
      }
      else
      {
        if (x == x->parent ()->left ())
        {
          // Transform case 2 into case 3 (see CLR book, pp. 269).
          x = x->parent ();
          RB_rotate_right (x);
        }

        // Handle case 3 (see CLR book, pp. 269).
        x->parent ()->color (ACE_RB_Tree_Node_Base::BLACK);
        x->parent ()->parent ()->color (ACE_RB_Tree_Node_Base::RED);
        RB_rotate_left (x->parent ()->parent ());
      }
    }
  }
}


// Method to find the successor node of the given node in the tree.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK> ACE_RB_Tree_Node<KEY, T> *
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::RB_tree_successor (ACE_RB_Tree_Node<KEY, T> *x) const
{
  if (x->right ())
  {
    return RB_tree_minimum (x->right ());
  }

  ACE_RB_Tree_Node<KEY, T> *y = x->parent ();
  while ((y) && (x == y->right ()))
  {
    x = y;
    y = y->parent ();
  }

  return y;
}


// Method to find the predecessor node of the given node in the tree.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK> ACE_RB_Tree_Node<KEY, T> *
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::RB_tree_predecessor (ACE_RB_Tree_Node<KEY, T> *x) const
{
  if (x->left ())
  {
    return RB_tree_maximum (x->left ());
  }

  ACE_RB_Tree_Node<KEY, T> *y = x->parent ();
  while ((y) && (x == y->left ()))
  {
    x = y;
    y = y->parent ();
  }

  return y;
}


// Method to find the minimum node of the subtree rooted at the given node.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK> ACE_RB_Tree_Node<KEY, T> *
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::RB_tree_minimum (ACE_RB_Tree_Node<KEY, T> *x) const
{
  while ((x) && (x->left ()))
  {
    x = x->left ();
  }

  return x;
}


// Method to find the maximum node of the subtree rooted at the given node.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK> ACE_RB_Tree_Node<KEY, T> *
ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK>::RB_tree_maximum (ACE_RB_Tree_Node<KEY, T> *x) const
{
  while ((x) && (x->right ()))
  {
    x = x->right ();
  }

  return x;
}




//////////////////////////////////////////////////////////
// template class ACE_RB_Tree_Iterator<KEY, T, COMPARE_KEYS, ACE_LOCK> //
//////////////////////////////////////////////////////////


// Constructor.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>
ACE_RB_Tree_Iterator<KEY, T, COMPARE_KEYS, ACE_LOCK>::ACE_RB_Tree_Iterator (const ACE_RB_Tree<KEY, T, COMPARE_KEYS, ACE_LOCK> &tree)
  : tree_ (tree), node_ (0)
{
  // Position the iterator at the first node in the tree.
  first ();
}


// Destructor.

template <class KEY, class T, class COMPARE_KEYS, class ACE_LOCK>
ACE_RB_Tree_Iterator<KEY, T, COMPARE_KEYS, ACE_LOCK>::~ACE_RB_Tree_Iterator ()
{
}


#endif /* !defined (ACE_RB_TREE_C) */
