// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Lazy_Map_Manager_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the <ACE_Map_Manager> and
//     <ACE_Active_Map_Manager> that illustrates how lazy map managers
//     allow the deletion of entries while iterating over the map.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>,
// ============================================================================

#include "test_config.h"
#include "ace/Map_Manager.h"
#include "ace/Active_Map_Manager.h"
#include "ace/Synch.h"
#include "ace/streams.h"

ACE_RCSID(tests, Map_Manager_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

// Simple map manager.
typedef ACE_Map_Manager<int, int, ACE_Null_Mutex> MAP;

// Displaying the contents of a map manager.

void
display_map (MAP &map)
{
  {
    // Simple iteration printing the entries.
    for (MAP::iterator iter = map.begin ();
         iter != map.end ();
         ++iter)
      {
        MAP::ENTRY &entry = *iter;
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("%d "),
                    entry.int_id_));
      }

    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
  }

  {
    // Simple reverse iteration printing the entries.
    for (MAP::reverse_iterator iter = map.rbegin ();
         iter != map.rend ();
         ++iter)
      {
        MAP::ENTRY &entry = *iter;
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("%d "),
                    entry.int_id_));
      }

    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("\n")));
  }

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\n")));
}

// Test for map manager.

void
map_test (void)
{
  // Map of size 3.
  MAP map (3);
  int i = 0;

  // Insert a few entries.
  for (i = 0; i < 3; ++i)
    map.bind (i, i);

  display_map (map);

  // Remove middle one.
  map.unbind (1);

  display_map (map);

  // Remove the entry on one end.
  map.unbind (0);

  display_map (map);

  // Remove the entry on the other end.
  map.unbind (2);

  display_map (map);

  // If we have lazy map managers, we can delete entries while
  // iterating over the map.

#if defined (ACE_HAS_LAZY_MAP_MANAGER)

  // Insert a few entries.
  for (i = 0; i < 3; ++i)
    map.bind (i, i);

  display_map (map);

  // Remove middle one.
  {
    // Deletion while iterating.
    for (MAP::iterator iter = map.begin ();
         iter != map.end ();
         ++iter)
    {
      MAP::ENTRY &entry = *iter;
      if (entry.int_id_ == 1)
        map.unbind (1);
    }

    display_map (map);
  }

  // Remove the entry on one end.
  {
    // Deletion while iterating.
    for (MAP::iterator iter = map.begin ();
         iter != map.end ();
         ++iter)
      {
        MAP::ENTRY &entry = *iter;
        if (entry.int_id_ == 0)
          map.unbind (0);
      }

    display_map (map);
  }

  // Remove the entry on the other end.
  {
    // Deletion while iterating.
    for (MAP::iterator iter = map.begin ();
         iter != map.end ();
         ++iter)
      {
        MAP::ENTRY &entry = *iter;
        if (entry.int_id_ == 2)
          map.unbind (2);
      }

    display_map (map);
  }

#endif /* ACE_HAS_LAZY_MAP_MANAGER */

  // Insert a few entries.  This will force an increase in map size.
  for (i = 0; i < 4; ++i)
    map.bind (i, i);

  display_map (map);

  // Remove a few entries (in reverse order).
  for (i = 3; i >= 0; --i)
    map.unbind (i);

  display_map (map);
}

// Simple active map manager.
typedef ACE_Active_Map_Manager<int> ACTIVE_MAP;

// Displaying the contents of an active map manager.

void
display_map (ACTIVE_MAP &map)
{
  {
    // Simple iteration printing the entries.
    for (ACTIVE_MAP::iterator iter = map.begin ();
         iter != map.end ();
         ++iter)
      {
        ACTIVE_MAP::ENTRY &entry = *iter;
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("%d "),
                    entry.int_id_));
      }

    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("\n")));
  }

  {
    // Simple reverse iteration printing the entries.
    for (ACTIVE_MAP::reverse_iterator iter = map.rbegin ();
         iter != map.rend ();
         ++iter)
      {
        ACTIVE_MAP::ENTRY &entry = *iter;
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("%d "),
                    entry.int_id_));
      }

    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("\n")));
  }

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\n")));
}

// Test for active map manager.

void
active_map_test (void)
{
  // Map of size 3.
  ACTIVE_MAP map (3);
  ACE_Active_Map_Manager_Key keys[4];
  int i = 0;

  // Insert a few entries.
  for (i = 0; i < 3; ++i)
    map.bind (i, keys[i]);

  display_map (map);

  // Remove middle one.
  map.unbind (keys[1]);

  display_map (map);

  // Remove the entry on one end.
  map.unbind (keys[0]);

  display_map (map);

  // Remove the entry on the other end.
  map.unbind (keys[2]);

  display_map (map);

  // If we have lazy map managers, we can delete entries while
  // iterating over the map.

#if defined (ACE_HAS_LAZY_MAP_MANAGER)

  // Insert a few entries.
  for (i = 0; i < 3; ++i)
    map.bind (i, keys[i]);

  display_map (map);

  // Remove middle one.
  {
    // Deletion while iterating.
    for (ACTIVE_MAP::iterator iter = map.begin ();
         iter != map.end ();
         ++iter)
    {
      ACTIVE_MAP::ENTRY &entry = *iter;
      if (entry.int_id_ == 1)
        map.unbind (keys[1]);
    }

    display_map (map);
  }

  // Remove the entry on one end.
  {
    // Deletion while iterating.
    for (ACTIVE_MAP::iterator iter = map.begin ();
         iter != map.end ();
         ++iter)
      {
        ACTIVE_MAP::ENTRY &entry = *iter;
        if (entry.int_id_ == 0)
          map.unbind (keys[0]);
      }

    display_map (map);
  }

  // Remove the entry on the other end.
  {
    // Deletion while iterating.
    for (ACTIVE_MAP::iterator iter = map.begin ();
         iter != map.end ();
         ++iter)
      {
        ACTIVE_MAP::ENTRY &entry = *iter;
        if (entry.int_id_ == 2)
          map.unbind (keys[2]);
      }

    display_map (map);
  }

#endif /* ACE_HAS_LAZY_MAP_MANAGER */

  // Insert a few entries.  This will force an increase in map size.
  for (i = 0; i < 4; ++i)
    map.bind (i, keys[i]);

  display_map (map);

  // Remove a few entries (in reverse order).
  for (i = 3; i >= 0; --i)
    map.unbind (keys[i]);

  display_map (map);
}

int
main (int, ASYS_TCHAR *)
{
  ACE_START_TEST (ASYS_TEXT ("Lazy_Map_Manager_Test"));
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\nMap Manager...\n\n")));
  map_test ();

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("\nActive Map Manager...\n\n")));
  active_map_test ();

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Map_Manager<int, int, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<int, int, ACE_Null_Mutex>;
template class ACE_Map_Iterator<int, int, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<int, int, ACE_Null_Mutex>;
template class ACE_Map_Entry<int, int>;

template class ACE_Active_Map_Manager<int>;
template class ACE_Map_Manager<ACE_Active_Map_Manager_Key, int, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_Active_Map_Manager_Key, int, ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_Active_Map_Manager_Key, int, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, int, ACE_Null_Mutex>;
template class ACE_Map_Entry<ACE_Active_Map_Manager_Key, int>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Map_Manager<int, int, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<int, int, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<int, int, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<int, int, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<int, int>

#pragma instantiate ACE_Active_Map_Manager<int>
#pragma instantiate ACE_Map_Manager<ACE_Active_Map_Manager_Key, int, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_Active_Map_Manager_Key, int, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_Active_Map_Manager_Key, int, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, int, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<ACE_Active_Map_Manager_Key, int>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
