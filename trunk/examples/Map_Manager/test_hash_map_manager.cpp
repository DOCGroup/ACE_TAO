// $Id$

#include "ace/Hash_Map_Manager.h"
#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Synch.h"

const int MAX_KEY_LEN = 1000;

typedef ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_RW_Mutex> MAP_MANAGER;

int
main (int argc, char *argv[])
{
  if (argc != 4)
    cerr << "usage: " << argv[0] << " tablesize file1 file2\n";
  else
    {
      int total = ACE_OS::atoi (argv[1]);

      if (!freopen (argv[2], "r", stdin))
        ACE_OS::perror (argv[0]), ACE_OS::exit (1);

      MAP_MANAGER hash (total);

      char key[MAX_KEY_LEN];

      while (ACE_OS::fgets (key, sizeof key, ACE_STDIN))
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
        perror (argv[0]), exit (1);

      while (ACE_OS::fgets (key, sizeof key, ACE_STDIN))
        {
          ACE_CString name (key);
          ACE_CString value;
          assert (hash.find (name, value) != -1);
          assert (name == value);
        }

      if (!freopen (argv[3], "r", stdin))
        perror (argv[0]), exit (1);

      while (ACE_OS::fgets (key, sizeof key, ACE_STDIN))
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
template class ACE_Guard<ACE_RW_Mutex>;
template class ACE_Read_Guard<ACE_RW_Mutex>;
template class ACE_Write_Guard<ACE_RW_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString, ACE_CString>;
template class ACE_Hash_Map_Iterator_Base<ACE_CString, ACE_CString, ACE_RW_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString, ACE_CString, ACE_RW_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_RW_Mutex>;
template class ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_RW_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Guard<ACE_RW_Mutex>
#pragma instantiate ACE_Read_Guard<ACE_RW_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_RW_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ACE_CString>
#pragma instantiate ACE_Hash_Map_Iterator_Base<ACE_CString, ACE_CString, ACE_RW_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString, ACE_CString, ACE_RW_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_RW_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_RW_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
