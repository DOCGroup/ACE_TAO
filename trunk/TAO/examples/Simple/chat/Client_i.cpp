// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Client_i.cpp
//
// = DESCRIPTION
//    Implementation of the Client_i class.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#include "Client_i.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"

Client_i::Client_i ()
  : ior_file_name_ ("chat.ior"),
    nickname_ ("noname")
{
}

Client_i::~Client_i (void)
{
  // Make sure to cleanup the STDIN handler.
  if (ACE_Event_Handler::remove_stdin_handler
      (TAO_ORB_Core_instance ()->reactor (),
       TAO_ORB_Core_instance ()->thr_mgr ()) == -1)
    ACE_ERROR ((LM_ERROR,
                       "%p\n",
                       "remove_stdin_handler"));
}

int
Client_i::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "n:f:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':  // get the users nickname
        this->nickname_ = get_opts.opt_arg ();
        break;

      case 'f':  // get the file name to write to
        this->ior_file_name_ = get_opts.opt_arg ();
        break;

      default: // display help for use of the serve
      case '?':  // display help for use of the server.
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-n <your_nick_name>]"
                           " [-f <ior_input_file>]"
                           "\n",
                           argv [0]),
                          -1);
      }

  ACE_DEBUG ((LM_DEBUG,
              "\nusing nickname = %s, filename = %s\n",
              this->nickname_,
              this->ior_file_name_));
  return 0;
}

int
Client_i::init (int argc, char *argv[])
{
  // Check if the command line arguments are ok.
  if (this->parse_args (argc, argv) == -1)
    return -1;

  ACE_TRY_NEW_ENV
    {
      // Retrieve the ORB.
      this->orb_manager_.init (argc,
                               argv,
                               0
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb = this->orb_manager_.orb ();

      // set the orb in the receiver_i_ object.
      this->receiver_i_.orb (orb.in ());

      // read the ior from file
      if (this->read_ior (this->ior_file_name_) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "could not read the ior from the file: <%s>\n",
                           this->ior_file_name_),
                          -1);

      CORBA::Object_var server_object =
        orb->string_to_object (this->ior_
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid ior <%s>\n",
                           this->ior_),
                          -1);

      this->server_ = Broadcaster::_narrow (server_object.in ()
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "client_i::init\n");
      return -1;
    }
  ACE_ENDTRY;

  // Register our <Input_Handler> to handle STDIN events, which will
  // trigger the <handle_input> method to process these events.

  if (ACE_Event_Handler::register_stdin_handler
      (this,
       TAO_ORB_Core_instance ()->reactor (),
       TAO_ORB_Core_instance ()->thr_mgr ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "register_stdin_handler"),
                      -1);
  return 0;
}

int
Client_i::run (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n============= Simple Chat =================\n"
              "========== type 'quit' to exit  ===========\n"));

  ACE_TRY_NEW_ENV
    {
      PortableServer::POAManager_var poa_manager =
        this->orb_manager_.poa_manager ();
      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->receiver_var_ =
        this->receiver_i_._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Register ourselves with the server.
      server_->add (this->receiver_var_.in (),
                    this->nickname_
                    TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Run the ORB.
      this->orb_manager_.run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Client_i::run ()");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Client_i::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  if (ACE_OS::fgets (buf, BUFSIZ, stdin) == 0)
    return 0;

  ACE_TRY_NEW_ENV
    {
      // Check if the user wants to quit.
      if (ACE_OS::strncmp (buf,
                           QUIT_STRING,
                           ACE_OS::strlen (QUIT_STRING)) == 0)
        {
          // Remove ourselves from the server.
          this->server_->remove (this->receiver_var_.in ());
          this->receiver_i_.shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);

          ACE_TRY_CHECK;
          return 0;
        }

      // Call the server function <say> to pass the string typed by
      // the server.
      this->server_->say (this->receiver_var_.in (),
                          buf
                          TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Input_Handler::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Client_i::read_ior (const char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename,
                       "invalid handle"),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior: %p\n"),
                      -1);

  this->ior_ = ACE_OS::strdup (data);
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  if (this->ior_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "failed to read ior from file\n",
                       ""),
                      -1);
  return 0;
}
