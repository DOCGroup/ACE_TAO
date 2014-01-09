
//=============================================================================
/**
 *  @file    Client_i.cpp
 *
 *  $Id$
 *
 *  Implementation of the Client_i class.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#include "Client_i.h"
#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"

Client_i::Client_i ()
  : ior_ ("")
  , ior_file_name_ (ACE_TEXT ("chat.ior"))
  , nickname_ ("noname")
{
  Receiver_i *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    Receiver_i (),
                    CORBA::NO_MEMORY ());
  this->receiver_i_ = tmp;
}

int
Client_i::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("n:f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'n':  // get the users nickname
        this->nickname_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        break;

      case 'f':  // get the file name to write to
        this->ior_file_name_ = get_opts.opt_arg ();
        break;

      default: // display help for use of the serve
      case '?':  // display help for use of the server.
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s")
                           ACE_TEXT (" [-n <your_nick_name>]")
                           ACE_TEXT (" [-f <ior_input_file>]")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nusing nickname = %C, filename = %s\n"),
              this->nickname_.c_str (),
              this->ior_file_name_));
  return 0;
}

int
Client_i::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Retrieve the ORB.
      this->orb_manager_.init (argc,
                               argv,
                               0);

      CORBA::ORB_var orb = this->orb_manager_.orb ();

      // Check if the command line arguments are ok.
      if (this->parse_args (argc, argv) == -1)
        return -1;

      // set the orb in the receiver_i_ object.
      this->receiver_i_->orb (orb.in ());

      // read the ior from file
      if (this->read_ior (this->ior_file_name_) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("could not read the ior from the file: <%s>\n"),
                           this->ior_file_name_),
                          -1);

      CORBA::Object_var server_object =
        orb->string_to_object (this->ior_.c_str ());

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("invalid ior <%C>\n"),
                           this->ior_.c_str ()),
                          -1);

      this->server_ = Broadcaster::_narrow (server_object.in ());
      if (CORBA::is_nil (this->server_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Nil Server\n")),
                            -1);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client_i::init\n");
      return -1;
    }

  return 0;
}

int
Client_i::run (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n============= Simple Chat =================\n")
              ACE_TEXT ("========== type 'quit' to exit  ===========\n")));

  try
    {
      PortableServer::POAManager_var poa_manager =
        this->orb_manager_.poa_manager ();
      poa_manager->activate ();

      this->receiver_var_ =
        this->receiver_i_->_this ();

      // Register ourselves with the server.
      server_->add (this->receiver_var_.in (),
                    this->nickname_.c_str ());

      // Register our <Input_Handler> to handle STDIN events, which will
      // trigger the <handle_input> method to process these events.
      if (ACE_Event_Handler::register_stdin_handler
          (this,
           TAO_ORB_Core_instance ()->reactor (),
           TAO_ORB_Core_instance ()->thr_mgr ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("register_stdin_handler")),
                          -1);

      // Run the ORB.
      this->orb_manager_.run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client_i::run ()");
      return -1;
    }

  return 0;
}

int
Client_i::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  if (ACE_OS::fgets (buf, BUFSIZ, stdin) == 0)
    return 0;

  try
    {
      // Check if the user wants to quit.
      if (ACE_OS::strncmp (buf,
                           QUIT_STRING,
                           ACE_OS::strlen (QUIT_STRING)) == 0)
        {
          // Remove ourselves from the server.
          try
            {
              this->server_->remove (this->receiver_var_.in ());
            }
          catch (const CORBA::Exception &)
            {
              // we don't care about problems
            }
          this->receiver_i_->shutdown ();

          return -1;
        }

      // Call the server function <say> to pass the string typed by
      // the server.
      this->server_->say (this->receiver_var_.in (),
                          buf);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Input_Handler::init");
      return -1;
    }

  return 0;
}

int
Client_i::read_ior (const ACE_TCHAR *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to open %s for writing (%p)\n"),
                       filename,
                       ACE_TEXT ("open")),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to read ior (%p)\n"),
                       ACE_TEXT ("read")),
                      -1);

  this->ior_ = data;
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  if (this->ior_.length () == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("failed to read ior from file\n")),
                      -1);
  return 0;
}
