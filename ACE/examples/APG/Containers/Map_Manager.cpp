// $Id$

#include "ace/Log_Msg.h"
#include "ace/Map_Manager.h"
#include "ace/Synch.h"
#include "DataElement.h"
#include "KeyType.h"

class Map_Example
{
public:
  // Illustrate the ACE_Map_Manager.
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

// Listing 2 code/ch05
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
      map_.find (j,d);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), d.getData ()));
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
// Listing 2
// Listing 3 code/ch05
void Map_Example::iterate_forward (void)
{
  ACE_TRACE ("Map_Example::iterate_forward");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Forward iteration\n")));
  for (ACE_Map_Manager<KeyType,
                       DataElement,
                       ACE_Null_Mutex>::iterator
       iter = map_.begin ();
       iter != map_.end ();
       iter++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"),
                  (*iter).int_id_.getData ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}


void Map_Example::iterate_reverse (void)
{
  ACE_TRACE ("Map_Example::iterate_reverse");
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Reverse iteration\n")));
  for (ACE_Map_Manager<KeyType,
                       DataElement,
                       ACE_Null_Mutex>::reverse_iterator
       iter = map_.rbegin ();
       iter != map_.end ();
       iter++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"),
                  (*iter).int_id_.getData ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}
// Listing 3
// Listing 4 code/ch05
void Map_Example::remove_all (void)
{
  ACE_TRACE ("Map_Example::remove_all");

  // Note that we can't use the iterators here as they
  // are invalidated after deletions or insertions.
  map_.unbind_all ();
}
// Listing 4

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  Map_Example me;
  return  me.run ();
}

