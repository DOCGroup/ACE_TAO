// $Id$

#include "ace/Hash_Map_Manager.h"
#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Synch.h"

ACE_RCSID(Map_Manager, test_hash_map_manager, "$Id$")

const int MAX_KEY_LEN = 1000;

typedef ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_SYNCH_RW_MUTEX> MAP_MANAGER;

int
main (int argc, ACE_TCHAR *argv[])
{
  if (argc != 4)
    ACE_ERROR_RETURN ((LM_ERROR, "usage: %s tablesize file1 file2\n",
                       argv[0]), -1);
  else
    {
      int total = ACE_OS::atoi (argv[1]);

      if (!freopen (argv[2], "r", stdin))
        ACE_OS::perror (argv[0]), ACE_OS::exit (1);

      MAP_MANAGER hash (total);

      char key[MAX_KEY_LEN];

      while (ACE_OS::fgets (key, sizeof key, stdin))
        // stream ops are just too slow!!
        {
          ACE_CString string (key);
          hash.bind (string, string);
        }

      fclose (stdin);

      MAP_MANAGER::ITERATOR iterator (hash);

      for (MAP_MANAGER::ENTRY *entry = 0;
           iterator.next (entry) != 0;
           iterator.advance ())
        ACE_DEBUG ((LM_DEBUG, "%s %s\n",
                    entry->ext_id_.fast_rep (),
                    entry->int_id_.fast_rep ()));

      if (!freopen (argv[3], "r", stdin))
        ACE_OS::perror (argv[0]), exit (1);

      while (ACE_OS::fgets (key, sizeof key, stdin))
        {
          ACE_CString name (key);
          ACE_CString value;
          assert (hash.find (name, value) != -1);
          assert (name == value);
        }

      if (!freopen (argv[3], "r", stdin))
        ACE_OS::perror (argv[0]), exit (1);

      while (ACE_OS::fgets (key, sizeof key, stdin))
        {
          ACE_CString string (key);
          assert (hash.unbind (string) != -1);
        }

      assert (hash.current_size () == 0);

      fclose (stdin);
    }

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_CString, ACE_CString>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator<ACE_CString, ACE_CString, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_RW_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#if defined(ACE_HAS_THREADS)
#pragma instantiate ACE_Guard<ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Read_Guard<ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Write_Guard<ACE_SYNCH_RW_MUTEX>
#endif /* ACE_HAS_THREADS */

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ACE_CString>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, ACE_CString, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_SYNCH_RW_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
