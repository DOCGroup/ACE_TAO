
//=============================================================================
/**
 *  @file     client.cpp
 *
 *    A client program for the File IDL module
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "FileC.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/SString.h"
#include "ace/Thread_Manager.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Malloc_Base.h"
#include <memory>
#include <string>
#include <utility>

struct Client_Options
{
  ACE_TString ior_file {ACE_TEXT ("ior")};
  ACE_TString file_name {ACE_TEXT ("big.txt")};
  int thread_count {4};
  int iterations {100};
};

struct Client_Context
{
  CORBA::ORB_var orb {};
  std::string ior;
  ACE_TString file_name;
  int iterations {};
};

struct Read_Buffer_Deleter
{
  ACE_Allocator *allocator;

  void operator() (char *data) const
  {
    this->allocator->free (data);
  }
};

static int
parse_args (int argc, ACE_TCHAR **argv, Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:dk:f:i:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        TAO_debug_level++;
        break;
      case 'k':
        options.ior_file = get_opts.opt_arg ();
        break;
      case 'f':
        options.file_name = get_opts.opt_arg ();
        break;
      case 't':
        options.thread_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'i':
        options.iterations = ACE_OS::atoi (get_opts.opt_arg ());
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

  // Indicates successful parsing of command line.
  return 0;
}

void
validate_connection (CORBA::Object_ptr object)
{
  // Try to validate the connection several times, ignoring transient
  // exceptions.  If the connection can still not be setup, return
  // failure.
  for (int i {}; i < 100; ++i)
    {
      try
        {
          object->_non_existent ();
        }
      catch (CORBA::TRANSIENT const &)
        {
          // Ignore...
        }
    }
}

static void *
MTTEST (void *args)
{
  Client_Context const &context = *static_cast<Client_Context *> (args);
  try
    {
      CORBA::Object_var const object = context.orb->string_to_object (context.ior.c_str ());

      validate_connection (object.in ());

      // Narrow the object reference to a File::System
      File::System_var const file_system = File::System::_narrow (object.in ());

      // Create the file filename i.e "test"
      File::Descriptor_var const fd = file_system->open (
        ACE_TEXT_ALWAYS_CHAR (context.file_name.c_str ()), O_RDONLY);

      for (int i {}; i < context.iterations; ++i)
        {
          //seek to the beginning of the file
#if 0
          ACE_DEBUG((LM_DEBUG,"Making request number %d\n",i));
#endif /*if 0*/

          fd->lseek (0, SEEK_SET);

          // Read back the written message
          // Twice the size of the socket buffer
          File::Descriptor::DataBuffer_var const data_received = fd->read (128 * 1024);
        }

      // close the file
      fd->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in main");
      return nullptr;
    }

  return nullptr;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Parse the command-line arguments to get the IOR
      int const result = parse_args (argc, argv, options);
      if (result != 0)
        return result;

      // Read the file, and get the IOR
      ACE_HANDLE const input_file = ACE_OS::open (options.ior_file.c_str (), 0);
      if (input_file == ACE_INVALID_HANDLE)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot open input file for reading IOR: %s\n"),
                           options.ior_file.c_str ()),
                          -1);
      ACE_Read_Buffer ior_buffer (input_file, true);
      std::unique_ptr<char, Read_Buffer_Deleter> data (
        ior_buffer.read (), Read_Buffer_Deleter {ior_buffer.alloc ()});
      if (!data)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to read ior\n"),
                          -1);

      std::string ior (data.get ());

      Client_Context context;
      context.orb = orb;
      context.ior = std::move (ior);
      context.file_name = options.file_name;
      context.iterations = options.iterations;

      if (ACE_Thread_Manager::instance ()->spawn_n (options.thread_count,
                                                    MTTEST,
                                                    &context,
                                                    THR_NEW_LWP | THR_DETACHED) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%p\n%a"),
                      ACE_TEXT ("thread create failed")));
        }
      ACE_Thread_Manager::instance ()->wait ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in main");
      return -1;
    }

  return 0;
}
