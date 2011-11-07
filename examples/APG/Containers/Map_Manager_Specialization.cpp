// $Id$

#include "ace/Log_Msg.h"
#include "ace/Map_Manager.h"
#include "ace/Synch.h" // Needed for the lock.
#include "DataElement.h"
#include "KeyType.h"

/*
** This needs to stay in the book for 2nd printing, but is the same as
** what's in KeyType.h.
*/
#if 0
// Listing 1 code/ch05
class KeyType
{
public:
  KeyType () : val_(0) {}

  KeyType (int i) : val_(i) {}

  KeyType (const KeyType& kt) { this->val_ = kt.val_; };

  operator int () const { return val_; };

private:
  int val_;
};

template<>
int
ACE_Map_Manager<KeyType, DataElement, ACE_Null_Mutex>::equal
(const KeyType& r1, const KeyType &r2)
{
  return (r1 == r2);
}
// Listing 1
#else
ACE_BEGIN_VERSIONED_NAMESPACE_DECL
template<>
int
ACE_Map_Manager<KeyType, DataElement, ACE_Null_Mutex>::equal
(const KeyType& r1, const KeyType &r2)
{
  return (r1 == r2);
}
ACE_END_VERSIONED_NAMESPACE_DECL
#endif /* 0 */

class Map_Example
{
public:
  // Illustrate the ACE_Map_Manager<>.
  int run (void);

private:
  // Iterate in the forward direction.
  void iterate_forward (void);

  // Iterate in the other direction.
  void iterate_reverse (void);

  // Remove all elements from the map.
  void remove_all (void);

private:
  ACE_Map_Manager<KeyType,DataElement,ACE_Null_Mutex> map_;
};

int Map_Example::run (void)
{
  ACE_TRACE ("Map_Example::run");

  // Corresponding KeyType objects are created on the fly.
  for (int i = 0; i < 100; i++)
    {
      map_.bind (i, DataElement (i));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Map has\n")));
  for (int j = 0; j < 100; j++)
    {
      DataElement d;
      int result = map_.find (j,d);
      if (result == 0)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), d.getData ()));
        }
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  // Iterate in the forward direction.
  this->iterate_forward ();

  // Iterate in the other direction.
  this->iterate_reverse ();

  // Remove all elements from the map.
  this->remove_all ();

  // Iterate in the forward direction.
  this->iterate_forward ();

  return 0;
}

void Map_Example::iterate_forward (void)
{
  ACE_TRACE ("Map_Example::iterate_forward");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Forward iteration\n")));
  for (ACE_Map_Manager<KeyType, DataElement, ACE_Null_Mutex>::iterator
       iter = map_.begin ();
       iter!= map_.end ();
       iter++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), (*iter).int_id_.getData ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}

void Map_Example::iterate_reverse (void)
{
  ACE_TRACE ("Map_Example::iterate_reverse");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Reverse iteration\n")));
  for (ACE_Map_Manager<KeyType, DataElement, ACE_Null_Mutex>::reverse_iterator
       iter = map_.rbegin ();
       iter!= map_.end ();
       iter++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), (*iter).int_id_.getData ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}

void Map_Example::remove_all (void)
{
  ACE_TRACE ("Map_Example::remove_all");

  // Note that we can't use the iterators here
  // as they are invalidated after deletions
  // or insertions.
  for (int i = 0; i < 100; i++)
    {
      map_.unbind (i);
    }
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  Map_Example me;
  return  me.run ();
}

