// ============================================================================
// $Id$

//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Map_Manager_Test.cpp
//
// = DESCRIPTION
//      This is a simple test of the ACE_Map_Manager that
//      illustrates how to use the forward and reverse iterators. 
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Map_Manager.h"
#include "ace/Synch.h"
#include "test_config.h"

typedef ACE_Null_Mutex MUTEX;
typedef int KEY;
typedef int VALUE;

typedef ACE_Map_Manager <KEY, VALUE, MUTEX> MAP_MANAGER;
typedef ACE_Map_Iterator <KEY, VALUE, MUTEX> ITERATOR;
typedef ACE_Map_Reverse_Iterator <KEY, VALUE, MUTEX> REVERSE_ITERATOR;
typedef ACE_Map_Entry <KEY, VALUE> ENTRY;

int 
main (int, char *argv[])
{
  ACE_START_TEST ("Map_Manager_Test.cpp");

  const int ITERATIONS = 5;
  MAP_MANAGER map;

  for (VALUE i = 1; i <= ITERATIONS; i++)
    {
      if (map.bind (i, i) != 0)
	ACE_ERROR_RETURN ((LM_ERROR, "MAP_MANAGER::bind"), -1);
    }

  ACE_DEBUG ((LM_DEBUG, "\nForward Iterations\n"));
  {
    ITERATOR iterator (map);  
    for (ENTRY *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      {
	ACE_DEBUG ((LM_DEBUG, "%d %d\n", entry->ext_id_, entry->int_id_));
      }  
  }

  ACE_DEBUG ((LM_DEBUG, "\nReverse Iterations\n"));
  {
    REVERSE_ITERATOR iterator (map);
    for (ENTRY *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      {
	ACE_DEBUG ((LM_DEBUG, "%d %d\n", entry->ext_id_, entry->int_id_));
      }  
  }

  ACE_DEBUG ((LM_DEBUG, "\nForward Iterations\n"));
  {
    MAP_MANAGER::ITERATOR iterator (map);
    for (MAP_MANAGER::ENTRY *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      {
	ACE_DEBUG ((LM_DEBUG, "%d %d\n", entry->ext_id_, entry->int_id_));
      }  
  }

  ACE_DEBUG ((LM_DEBUG, "\nReverse Iterations\n"));
  {
    MAP_MANAGER::REVERSE_ITERATOR iterator (map);
    for (MAP_MANAGER::ENTRY *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      {
	ACE_DEBUG ((LM_DEBUG, "%d %d\n", entry->ext_id_, entry->int_id_));
      }  
  }

  ACE_END_TEST;
  return 0;
}
