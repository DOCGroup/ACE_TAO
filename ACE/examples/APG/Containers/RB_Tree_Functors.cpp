// $Id$

#include "ace/Log_Msg.h"
#include "DataElement.h"
#include "RB_Tree_Functors.h"

// Listing 0 code/ch05
#include "ace/RB_Tree.h"
#include "ace/Synch.h"

// Little helper class.
template<class EXT_ID, class INT_ID>
class Tree : public ACE_RB_Tree<EXT_ID, INT_ID,
                                ACE_Less_Than<EXT_ID>,
                                ACE_Null_Mutex>
{};
// Listing 0

class Tree_Example
{
public:
  // Illustrate the tree.
  int run (void);

private:
  // Use the forward iterator.
  void iterate_forward (void);

  // Use the reverse iterator.
  void iterate_reverse (void);

  // Remove all elements from the tree.
  int remove_all (void);

private:
  Tree<KeyType, DataElement*> tree_;
};


int Tree_Example::run ()
{
  ACE_TRACE ("Tree_Example::run");

  DataElement *d  = 0;
  for (int i = 0; i < 100; i++)
    {
      ACE_NEW_RETURN (d, DataElement (i), -1);
      int result = tree_.bind(i, d);
      if (result != 0)
        {
          ACE_ERROR_RETURN((LM_ERROR, "%p\n", "Bind"), -1);
        }
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Using find:\n")));
  for (int j = 0; j < 100; j++)
    {
      DataElement* d = 0;
      int result = tree_.find (j, d);
      if (result != 0)
        {
          ACE_ERROR_RETURN((LM_ERROR, "%p\n", "Find"), -1);
        }
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), d->getData ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  // Use the forward iterator.
  this->iterate_forward ();

  // Use the reverse iterator.
  this->iterate_reverse ();

  // Remove all elements from the tree.
  ACE_ASSERT (this->remove_all ()!= -1);

  // Iterate through once again.
  this->iterate_forward ();

  return 0;
}

void Tree_Example::iterate_forward (void)
{
  ACE_TRACE ("Tree_Example::iterate_forward");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Forward Iteration\n")));
  for (Tree<KeyType, DataElement*>::iterator iter = tree_.begin ();
       iter != tree_.end (); iter++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), (*iter).item ()->getData ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}

void Tree_Example::iterate_reverse (void)
{
  ACE_TRACE ("Tree_Example::iterate_reverse");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Reverse Iteration\n")));
  for (Tree<KeyType, DataElement*>::reverse_iterator iter = tree_.rbegin ();
       iter != tree_.rend (); iter++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), (*iter).item ()->getData ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}

int Tree_Example::remove_all (void)
{
  ACE_TRACE ("Tree_Example::remove_all");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Removing elements\n")));
  for (int i = 0; i < 100; i++)
    {
      DataElement * d = 0;
      int result = tree_.unbind (i, d);
      if (result != 0)
        {
          ACE_ERROR_RETURN((LM_ERROR, "%p\n", "Unbind"), -1);
        }
      ACE_ASSERT (d != 0);
      delete d;
    }

  return 0;
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  Tree_Example te;
  return te.run ();
}

