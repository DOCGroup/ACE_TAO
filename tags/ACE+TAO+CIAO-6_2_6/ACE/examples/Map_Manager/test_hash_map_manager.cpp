// $Id$

#include "ace/OS_main.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Synch.h"
#include "ace/Synch_Traits.h"
#include "ace/OS_NS_stdio.h"
#include "ace/os_include/os_assert.h"



const int MAX_KEY_LEN = 1000;

typedef ACE_Hash_Map_Manager<ACE_TString, ACE_TString, ACE_SYNCH_RW_MUTEX> MAP_MANAGER;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc != 4)
    ACE_ERROR_RETURN ((LM_ERROR, "usage: %s tablesize file1 file2\n",
                       argv[0]), -1);
  else
    {
      int total = ACE_OS::atoi (argv[1]);

      if (!ACE_OS::freopen (argv[2], ACE_TEXT("r"), stdin))
        ACE_OS::perror (argv[0]), ACE_OS::exit (1);

      MAP_MANAGER hash (total);

      ACE_TCHAR key[MAX_KEY_LEN];

      while (ACE_OS::fgets (key, sizeof key, stdin))
        // stream ops are just too slow!!
        {
          ACE_TString string (key);
          hash.bind (string, string);
        }

      ACE_OS::fclose (stdin);

      MAP_MANAGER::ITERATOR iterator (hash);

      for (MAP_MANAGER::ENTRY *entry = 0;
           iterator.next (entry) != 0;
           iterator.advance ())
        ACE_DEBUG ((LM_DEBUG, "%s %s\n",
                    entry->ext_id_.fast_rep (),
                    entry->int_id_.fast_rep ()));

      if (!ACE_OS::freopen (argv[3], ACE_TEXT("r"), stdin))
        ACE_OS::perror (argv[0]), ACE_OS::exit (1);

      while (ACE_OS::fgets (key, sizeof key, stdin))
        {
          ACE_TString name (key);
          ACE_TString value;
          assert (hash.find (name, value) != -1);
          assert (name == value);
        }

      if (!ACE_OS::freopen (argv[3], ACE_TEXT("r"), stdin))
        ACE_OS::perror (argv[0]), ACE_OS::exit (1);

      while (ACE_OS::fgets (key, sizeof key, stdin))
        {
          ACE_TString string (key);
          assert (hash.unbind (string) != -1);
        }

      assert (hash.current_size () == 0);

      ACE_OS::fclose (stdin);
    }

  return 0;
}

