
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
#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/SString.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"

#include <cstring>

struct Client_Options
{
  ACE_TCHAR const *ior_file {};
  ACE_TCHAR const *filename { ACE_TEXT ("test") };
  ACE_TCHAR const *message { ACE_TEXT ("POA rules!!") };
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("dk:f:m:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'd':
        ++TAO_debug_level;
        break;
      case 'k':
        options.ior_file = get_opts.opt_arg ();
        break;
      case 'f':
        options.filename = get_opts.opt_arg ();
        break;
      case 'm':
        options.message = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           "[-k <iorfile>]"
                           "[-f <filename>]"
                           "[-m <message>]"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (options.ior_file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      Client_Options options;
      // Parse the command-line arguments to get the IOR
      if (parse_args (argc, argv, options) != 0)
        return -1;

      // Read the file, and get the IOR
      ACE_HANDLE const input_file = ACE_OS::open (options.ior_file, 0);
      if (input_file == ACE_INVALID_HANDLE)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open input file %s for reading IOR\n",
                           options.ior_file),
                          -1);
      ACE_Read_Buffer ior_buffer (input_file);
      char *data = ior_buffer.read ();
      if (data == nullptr)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to read ior\n"),
                          -1);

      ACE_CString const ior = data;
      ior_buffer.alloc ()->free (data);
      ACE_OS::close (input_file);

      CORBA::Object_var object = orb->string_to_object (ior.c_str ());

      // Narrow the object reference to a File::System
      File::System_var file_system = File::System::_narrow (object.in ());

      // Creat the file filename i.e "test"
      File::Descriptor_var fd = file_system->open (ACE_TEXT_ALWAYS_CHAR (options.filename),
                                                   O_CREAT | O_RDWR);

      int const message_length = std::strlen (ACE_TEXT_ALWAYS_CHAR (options.message)) + 1;
      CORBA::Octet *buffer = File::Descriptor::DataBuffer::allocbuf (message_length);
      std::strcpy (reinterpret_cast<char *> (buffer), ACE_TEXT_ALWAYS_CHAR (options.message));
      File::Descriptor::DataBuffer data_sent (message_length, message_length, buffer, 1);

      // write the message to the file
      fd->write (data_sent);

      // seek to the beginning of the file
      fd->lseek (0, SEEK_SET);

      // Read back the written message
      File::Descriptor::DataBuffer_var data_received = fd->read (message_length);

      char *result = reinterpret_cast<char *> (&data_received[0]);

      // print the read message
      ACE_DEBUG ((LM_DEBUG, "%C\n",
                 result));

      // close the file
      fd->destroy ();

      file_system->shutdown ();

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in main");
      return -1;
    }

  return 0;
}
