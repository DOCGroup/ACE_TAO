// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Purge_Test.cpp
//
// = DESCRIPTION
//    This test illustrates the use of the new purging feature
//    which is incorporated in the Hash_Purgable_Map_Manager class.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "ace/Hash_Purgable_Map_Manager_T.h"
#include "test_config.h" 
#include "ace/Containers.h"
#include "ace/OS.h"
#include "ace/Functor_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

ACE_RCSID(tests, Purgable_Map_Manager_Test, "$Id$")

class Entry_ID 
{
  //= TITLE
  //   This class is a test class which contains an integer value.
  //
  //= DESCRIPTION
  //   This class is used as an KEY and VALUE of the map entry.
  
public:
  Entry_ID (int val = 0)
    : val_ (val)
    {
    }
  
  ~Entry_ID (void)
    {
    }

  // Obtain the value stored.
  int get_val (void)
    {
      return this->val_;
    }

  // Hashing needed to access an entry in the map.
  u_long hash (void) const
    {
      return this->val_;
    } 
  // Needed since every entry KEY has to be checked on a lookup.
   friend int operator== (const Entry_ID &lhs, const Entry_ID &rhs) 
    {
      return (&lhs == &rhs) ? 0 : 1;
    }
  
private:   
  int val_;
};

// The map which has the purging feature. 
typedef ACE_Hash_Purgable_Map_Manager_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex> 
         PURGE_MAP;

// Function to display the entries in the map.
void display_map (PURGE_MAP &map)
{
  // The Hash_Map entry without the purgable feature.
  typedef ACE_Hash_Map_Entry<Entry_ID, Entry_ID> *ENTRY;

  // The derived class from Hash_Map_Entry which facilitates purging thru its purge_tag member.
  typedef ACE_Hash_Purgable_Map_Entry<Entry_ID, Entry_ID> *PURGABLE_ENTRY;

  ACE_DEBUG ((LM_DEBUG,
              "Entries in the map:\n"));
  
  ENTRY entry = 0;
  PURGABLE_ENTRY purgable_entry = 0;
  
  // Simply traverse thru the map, displaying the entries.
  for (PURGE_MAP::iterator iter (map);
       iter.next (entry) != 0;
       iter.advance ())
    {      
      purgable_entry = (PURGABLE_ENTRY) entry;

     ACE_DEBUG ((LM_DEBUG,
                 "%d:%d purge_tag:%d\n",
                 (purgable_entry->ext_id_).get_val (),
                 (purgable_entry->int_id_).get_val (),
                 purgable_entry->get_purge_tag ()));
      
    }
}

int main (int argc, char *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST ("Purgable_Map_Manager_Test");

  // The map object.
  PURGE_MAP map;

  // The EXT_ID elements of the entry.
  Entry_ID addr1 (1234), addr2 (5678), addr3 (678), addr4 (12);
  // The INT_ID elements of the entry.
  Entry_ID ts1 (7), ts2 (8), ts3 (0), ts4 (0);
  
  ACE_ASSERT (map.bind (addr1, ts1) == 0);

  ACE_ASSERT (map.bind (addr2, ts2) == 0);

  ACE_ASSERT (map.bind (addr3, ts3) == 0);

  ACE_ASSERT (map.bind (addr4, ts4) == 0);

  // Display entries in the map.
  display_map (map);

  ACE_DEBUG ((LM_DEBUG,
              "Lookup entry %d in the map...\n", addr1.get_val ()));
  
  // Lookup an entry in the map.
  ACE_ASSERT (map.find (addr1) == 0);

  ACE_DEBUG ((LM_DEBUG,
              "Note the change in the purge_tag value of the entry looked up...\n"));
  
  // Display entries in the map. There would be a change in the
  // purge_tag values now.
  display_map (map);
  ACE_DEBUG ((LM_DEBUG,
              "current size = %d\n",map.current_size ()));
  // Map is full! need to purge. Decided to purge two of them.
  ACE_ASSERT (map.purge (2) == 0);

  ACE_DEBUG ((LM_DEBUG,
              "Purging two least recently used entries from map...\nHence the "));
  
  // Display entries in the map. There would be a change in the
  // purge_tag values now.
  display_map (map);
  ACE_DEBUG ((LM_DEBUG,
              "current size = %d\n",map.current_size ()));
  ACE_END_TEST;
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Purgable_Map_Entry<Entry_ID, Entry_ID>;
template class ACE_Equal_To<Entry_ID>;
template class ACE_Hash<Entry_ID>;
template class ACE_Hash_Purgable_Map_Manager_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<Entry_ID, Entry_ID>;
template class ACE_Hash_Map_Manager_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Purgable_Map_Entry<Entry_ID, Entry_ID >
#pragma instantiate ACE_Equal_To<Entry_ID>
#pragma instantiate ACE_Hash<Entry_ID>
#pragma instantiate ACE_Hash_Purgable_Map_Manager_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<Entry_ID, Entry_ID >
#pragma instantiate ACE_Hash_Map_Manager_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<Entry_ID, Entry_ID, ACE_Hash<Entry_ID>, ACE_Equal_To<Entry_ID>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
