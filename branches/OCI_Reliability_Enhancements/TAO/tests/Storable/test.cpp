// $Id$

// Writes/reads a string to file multiple times with a
// sleep after each write. Indented to be ran in
// multiple processes to verify file locking is done
// properly.

#include "Savable.h"

#include "tao/Storable_FlatFileStream.h"

#include "ace/Get_Opt.h"

#include <iostream>

const ACE_TCHAR *persistence_file = ACE_TEXT("test.dat");

int num_loops = 10;
int sleep_msecs = 100;
int string_index = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:s:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':
        num_loops = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 's':
        sleep_msecs = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        string_index = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-n <number-of-loops>"
                           "-s <milliseconds-to-sleep-in-loop>"
                           "-i <0 to modify string1, 1 to modify string2>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) != 0)
    return 1;

  TAO::Storable_FlatFileFactory factory ("./");

  Savable savable(factory);

  ACE_Time_Value sleep_time (0, 1000*sleep_msecs);

  char index_str[2];
  ACE_OS::sprintf (index_str, "%d", string_index);
  ACE_CString str_write = ACE_CString("string") + ACE_CString(index_str);
  ACE_CString str_read;

  for (int j = 0; j < num_loops; ++j)
    {
      savable.string_set(string_index, str_write);
      ACE_OS::sleep (sleep_time);
      str_read = savable.string_get (string_index);
      ACE_ASSERT (str_write == str_read);
    }

  return 0;
}
