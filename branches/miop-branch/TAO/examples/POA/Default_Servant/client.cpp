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
#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS.h"

ACE_RCSID(Default_Servant, client, "$Id$")

static const char *iorfile = 0;
static const char *filename = "test";
static const char *message = "POA rules!!";

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "dk:f:m:");
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
      case 'm':
        message = get_opts.opt_arg ();
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

  if (iorfile == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Please specify the IOR for the servant"), -1);

  // Indicates successful parsing of command line.
  return 0;
}

int
main (int argc, char **argv)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
      ior_buffer.alloc ()-> free (data);
      ACE_OS::close (input_file);

      CORBA::Object_var object = orb->string_to_object (ior.c_str ()
                                                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow the object reference to a File::System
      File::System_var file_system = File::System::_narrow (object.in ()
                                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Creat the file filename i.e "test"
      File::Descriptor_var fd = file_system->open (filename,
                                                   O_CREAT | O_RDWR
                                                   TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      int message_length = ACE_OS::strlen (message) + 1;
      CORBA::Octet *buffer = File::Descriptor::DataBuffer::allocbuf (message_length);
      ACE_OS::strcpy ((char *) buffer, message);
      File::Descriptor::DataBuffer data_sent (message_length, message_length, buffer, 1);

      // write the message to the file
      fd->write (data_sent TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //seek to the beginning of the file
      fd->lseek (0, SEEK_SET TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Read back the written message
      File::Descriptor::DataBuffer_var data_received = fd->read (message_length
                                                                 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      char *result = (char *) &data_received[0];

      // print the read message
      ACE_DEBUG((LM_DEBUG,"%s\n",
                 result));

      // close the file
      fd->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception caught in main");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}
