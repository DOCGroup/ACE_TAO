#include "ace/ACE.h"
#include "ace/Synch.h"
#include "ace/Hash_Map_Manager.h"

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

      while (gets (key))  // stream ops are just too slow!!
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

      while (gets (key)) 
	{
	  ACE_CString name (key);
	  ACE_CString value;
	  assert (hash.find (name, value) != -1);
	  assert (name == value);
	}

      if (!freopen (argv[3], "r", stdin)) 
	perror (argv[0]), exit (1);

      while (gets (key)) 
	{
	  ACE_CString string (key);
	  assert (hash.unbind (string) != -1);
	}

      assert (hash.current_size () == 0);

      fclose (stdin);
    }

  return 0;
}
