// $Id$

// Assumed to be one of two processes that
// writes/reads from persistent store.
// Each process writes one group of attributes
// and reads from the group written by the
// other process. After reading a check is
// made that expected values are read.
// A sleep is done at the end of the loop
// to account for limitation in resolution
// of timestamp on file used to determine
// if the file is stale.

#include "Savable.h"

#include "tao/Storable_FlatFileStream.h"
#include "tao/SystemException.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

#include <iostream>

const ACE_TCHAR *persistence_file = ACE_TEXT("test.dat");

int num_loops = 1;
int sleep_msecs = 100;
int write_index = 0;
bool use_backup = false;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("n:s:i:b"));
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
        write_index = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'b':
        use_backup = true;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("usage:  %s ")
                           ACE_TEXT("-n <number-of-loops> ")
                           ACE_TEXT("-s <milliseconds-to-sleep-in-loop> ")
                           ACE_TEXT("-i <index-used-for-writing> ")
                           ACE_TEXT("-b (use backup) ")
                           ACE_TEXT("\n"),
                           argv [0]),
                          -1);
      }

  if (write_index != 0 && write_index != 1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT("Error: Value passed to -i should be ")
                         ACE_TEXT("0 or 1.")),
                         -1);
    }

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int exit_status = 0;

  if (parse_args (argc, argv) != 0)
    return 1;

  int read_index = write_index ? 0 : 1;

  TAO::Storable_Base::use_backup_default = use_backup;

  TAO::Storable_FlatFileFactory factory ("./");

  ACE_CString str_write_value = "test_string";
  int int_write_value = -100;
  unsigned int unsigned_int_write_value = 100;

  const int bytes_size = 8;
  char bytes_write_value[bytes_size];
  char bytes_read[bytes_size];
  // Set 1st byte to 32 (ASCII space) to
  // make sure fscanf doesn't consume it
  // when reading the size.
  bytes_write_value[0] = 32;
  for (int i = 1; i < bytes_size; ++i)
    {
      bytes_write_value[i] = i;
    }

  ACE_Time_Value sleep_time (0, 1000*sleep_msecs);

  try
    {
      for (int j = 0; j < num_loops; ++j)
        {

          // Constructor called num_loops times.
          // Each time state read from persistent store.
          Savable savable(factory);

          // If the file was read, verify what was
          // written from other process is correct.
          if (savable.is_loaded_from_stream ())
            {

              int int_read = savable.int_get(read_index);
              // If value read is not 0 then the other
              // process has written to persistent store.
              // If not, it's too soon to test.
              if (int_read != 0)
                {
                  ACE_ASSERT (int_read == int_write_value);

                  const ACE_CString & str_read = savable.string_get(read_index);
                  if (str_read != str_write_value)
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("String read != num written.\n")));

                  unsigned int unsigned_int_read =
                    savable.unsigned_int_get (read_index);
                  if (unsigned_int_read != unsigned_int_write_value)
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("Int read != num written.\n")));

                  int bytes_read_size =
                    savable.bytes_get (read_index, bytes_read);
                  if (bytes_read_size != bytes_size)
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("Bytes read != num written.\n")));

                  for (int k = 0; k < bytes_size; ++k)
                    {
                      ACE_ASSERT (bytes_read[k] == bytes_write_value[k]);
                    }
                }
            }

          // Write out state
          savable.string_set(write_index, str_write_value);
          savable.int_set(write_index, int_write_value);
          savable.unsigned_int_set(write_index, unsigned_int_write_value);
          savable.bytes_set(write_index, bytes_size, bytes_write_value);
          int bytes_size = savable.bytes_get (write_index, bytes_read);
          for (int k = 0; k < bytes_size; ++k)
            {
              ACE_ASSERT (bytes_read[k] == bytes_write_value[k]);
            }

          ACE_OS::sleep (sleep_time);
        }
    }

  catch (Savable_Exception &)
    {
      std::cout << "Savable_Exception thrown" << std::endl;
      exit_status = 1;
    }

  catch (TAO::Storable_Read_Exception &ex)
    {
      std::cout << "TAO::Storable_Read_Exception thrown with state " <<
        ex.get_state () << std::endl;
      exit_status = 1;
    }

  catch (TAO::Storable_Write_Exception &ex)
    {
      std::cout << "TAO::Storable_Write_Exception thrown with state " <<
        ex.get_state () << std::endl;
      exit_status = 1;
    }

  catch (CORBA::PERSIST_STORE &)
    {
      std::cout << "CORBA::PERSIST_STORE thrown" << std::endl;
      exit_status = 1;
    }

  return exit_status;
}
