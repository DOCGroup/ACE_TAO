// $Id$

#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h" // Needed for the lock
#include "ace/Functor.h"
#include "DataElement.h"
#include "Hash_Map_Hash.h"

// Little helper class
template<class EXT_ID, class INT_ID>
class Hash_Map :
      public ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID,
      ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_Null_Mutex>
{};


class Hash_Map_Example
{
public:
  ~Hash_Map_Example ()
  {
    map_.close ();
  }

  // illustrate the hash map
  int run (void);

  // use the forward iterate
  void iterate_forward (void);

  // use the reverse iterator
  void iterate_reverse (void);

  // remove all the elements from the map
  void remove_all (void);

private:
  Hash_Map<KeyType, DataElement> map_;
};

int Hash_Map_Example::run (void)
{
  ACE_TRACE ("Hash_Map_Example::run");

  for (int i = 0; i < 100; i++)
    {
      map_.bind (i, DataElement (i));
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Map has\n")));
  for (int j = 0; j < 100; j++)
    {
      DataElement d;
      map_.find (j, d);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), d.getData ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  // Use the forward iterator.
  this->iterate_forward ();

  // Use the reverse iterator.
  this->iterate_reverse ();

  // Remove all the elements from the map.
  this->remove_all ();

  // Iterate through the map again.
  this->iterate_forward ();

  return 0;
}

void Hash_Map_Example::iterate_forward (void)
{
  ACE_TRACE ("Hash_Map_Example::iterate_forward");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Forward Iteration\n")));
  for (Hash_Map<KeyType, DataElement>::iterator iter = map_.begin ();
       iter != map_.end (); iter++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), (*iter).int_id_.getData ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}

void Hash_Map_Example::iterate_reverse (void)
{
  ACE_TRACE ("Hash_Map_Example::iterate_reverse");

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Reverse Iteration\n")));
  for (Hash_Map<KeyType, DataElement>::reverse_iterator iter = map_.rbegin ();
       iter != map_.rend (); iter++)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d:"), (*iter).int_id_.getData ()));
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}

void Hash_Map_Example::remove_all (void)
{
  ACE_TRACE ("Hash_Map_Example::remove_all");
  map_.unbind_all ();
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  Hash_Map_Example me;
  return me.run ();
}

