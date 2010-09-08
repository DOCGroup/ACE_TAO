// $Id$

//===================================================================
//  = LIBRARY
//      TAO/tests/POA/Default_Servant/client
//
//  = FILENAME
//     client.cpp
//
//  = DESCRIPTION
//      A client program for the File IDL module
//
//  = AUTHOR
//     Irfan Pyarali
//
//====================================================================

#include "FileC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS.h"
#include "ace/SString.h"
#include "ace/Thread_Manager.h"

static const ACE_TCHAR *iorfile = ACE_TEXT("ior");
static const ACE_TCHAR *filename = ACE_TEXT("big.txt");

static int NUM_THREADS = 4;
static int iterations = 100;

static CORBA::ORB_var orb;

static int
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:dk:f:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        TAO_debug_level++;
        break;
      case 'k':
        iorfile = get_opts.opt_arg ();
        break;
      case 'f':
        filename = get_opts.opt_arg ();
        break;
      case 't':
        NUM_THREADS = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        iterations = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "[-k <iorfile>]"
                           "[-f <filename>]"
                           "[-m <message>]"
                           "[-t <threads>]"
                           "[-i <iterations>]"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (iorfile == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

void
validate_connection (CORBA::Object_ptr object)
{
  // Try to validate the connection several times, ignoring transient
  // exceptions.  If the connection can still not be setup, return
  // failure.
  for (int i = 0; i < 100; ++i)
    {
      try
        {
          object->_non_existent ();
        }
      catch (const CORBA::TRANSIENT& )
        {
          // Ignore...
        }
      catch (const CORBA::Exception&)
        {
          // Rethrow any other exceptions.
          throw;
        }
    }
}

static void *
MTTEST (void *args)
{

  ACE_CString &ior = *(ACE_CString*)args;
  try
    {
      CORBA::Object_var object = orb->string_to_object (ior.c_str ());

      validate_connection (object.in ());

      // Narrow the object reference to a File::System
      File::System_var file_system = File::System::_narrow (object.in ());

      // Create the file filename i.e "test"
      File::Descriptor_var fd = file_system->open (ACE_TEXT_ALWAYS_CHAR(filename),
                                                   O_RDONLY);

      for( int i = 0; i < iterations; ++i)
        {
          //seek to the beginning of the file
#if 0
          ACE_DEBUG((LM_DEBUG,"Making request number %d\n",i));
#endif /*if 0*/

          fd->lseek (0, SEEK_SET);

          // Read back the written message
          // Twice the size of the socket buffer
          File::Descriptor::DataBuffer_var data_received = fd->read (128*1024);
        }

      // close the file
      fd->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in main");
      return 0;
    }

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  try
    {
      // Initialize the ORB
      orb = CORBA::ORB_init (argc, argv);

      // Parse the command-line arguments to get the IOR
      parse_args (argc, argv);

      // parse args should catch this, but just in case...
      if (iorfile == 0)
        return 0;

      // Read the file, and get the IOR
      ACE_HANDLE input_file = ACE_OS::open (iorfile, 0);
      if (input_file == ACE_INVALID_HANDLE)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open input file for reading IOR: %s\n",
                           iorfile),
                          -1);
      ACE_Read_Buffer ior_buffer (input_file);
      char *data = ior_buffer.read ();
      if (data == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to read ior\n"),
                          -1);

      ACE_CString ior = data;
      ior_buffer.alloc ()->free (data);
      ACE_OS::close (input_file);

      if (ACE_Thread_Manager::instance ()->spawn_n (NUM_THREADS,
                                                    ACE_THR_FUNC (MTTEST),
                                                    &ior,
                                                    THR_NEW_LWP | THR_DETACHED) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n%a"),
                      ACE_TEXT ("thread create failed")));
        }
      ACE_Thread_Manager::instance()->wait();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in main");
      return -1;
    }

  return 0;
}
