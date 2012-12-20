// $Id$

// Writes/reads a string to file multiple times with a
// sleep after each write. Indented to be ran in
// multiple processes to verify file locking is done
// properly.

#include "Savable.h"

#include "tao/Storable_FlatFileStream.h"

#include "ace/Get_Opt.h"

#include "ace/OS_NS_unistd.h"

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

  const int bytes_size = 8;
  char * bytes_set = new char [bytes_size];
  char * bytes_get = new char [bytes_size];
  // Set 1st byte to 32 (ASCII space) to
  // make sure fscanf doesn't consume it
  // when reading the size.
  bytes_set[0] = 32;
  for (int i = 1; i < bytes_size; ++i)
    {
      bytes_set[i] = i;
    }

  try
    {
      Savable savable(factory);

      // In case the file was read, bytes better match up
      if (savable.is_loaded_from_stream ())
        {
          int bytes_read_size = savable.bytes_get (bytes_get);
          ACE_ASSERT (bytes_read_size == bytes_size);
          for (int k = 0; k < bytes_size; ++k)
            {
              ACE_ASSERT (bytes_get[k] == bytes_set[k]);
            }
        }

      ACE_Time_Value sleep_time (0, 1000*sleep_msecs);

      char index_str[2];
      ACE_OS::sprintf (index_str, "%d", string_index);
      ACE_CString str_write = ACE_CString("string") + ACE_CString(index_str);
      ACE_CString str_read;

      for (int j = 0; j < num_loops; ++j)
        {
          savable.string_set(string_index, str_write);
          savable.int_set(j);
          savable.bytes_set(bytes_size, bytes_set);
          ACE_OS::sleep (sleep_time);
          str_read = savable.string_get (string_index);
          ACE_ASSERT (str_write == str_read);
          int bytes_read_size = savable.bytes_get (bytes_get);
          ACE_ASSERT (bytes_read_size == bytes_size);
          for (int k = 0; k < bytes_size; ++k)
            {
              ACE_ASSERT (bytes_get[k] == bytes_set[k]);
            }
        }
    }

  catch (Storable_Exception &ex)
    {
      std::cout << "Storable_Exception thrown with state " <<
        ex.get_state () << std::endl;
    }

  delete [] bytes_set;
  delete [] bytes_get;

  return 0;
}
