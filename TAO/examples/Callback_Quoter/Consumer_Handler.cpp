
//=============================================================================
/**
 *  @file    Consumer_Input_Handler.cpp
 *
 *  $Id$
 *
 *  Implementation of the Consumer_Handler class.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "Consumer_Handler.h"

#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"

#include "ace/Read_Buffer.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/OS_NS_fcntl.h"

Consumer_Handler::Consumer_Handler (void)
  : stock_name_ ("Unknown"),
    threshold_value_ (0),
    server_ (),
    registered_ (0),
    unregistered_ (0),
    ior_ (0),
    shutdown_ (0),
    use_naming_service_ (1),
    interactive_ (1)
{

}

Consumer_Handler::~Consumer_Handler (void)
{
  // Make sure to cleanup the STDIN handler.

  if (this->interactive_ == 1)
    {
      if (ACE_Event_Handler::remove_stdin_handler
          (this->orb_->orb_core ()->reactor (),
           this->orb_->orb_core ()->thr_mgr ()) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p\n",
                    "remove_stdin_handler"));
    }
}

// Reads the Server factory IOR from a file.

int
Consumer_Handler::read_ior (ACE_TCHAR *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for reading: %p\n",
                       filename),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior: %p\n"),
                      -1);

  this->ior_ = ACE_OS::strdup (ACE_TEXT_CHAR_TO_TCHAR(data));
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  return 0;
}

// Parses the command line arguments and returns an error status.

int
Consumer_Handler::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("a:t:d:f:xk:xs"));
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++; //****
        break;

      case 'k':  // ior provide on command line
        this->ior_ = ACE_OS::strdup (get_opts.opt_arg ());
        break;

      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.opt_arg ());
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);
        break;

      case 's': // don't use the naming service
        this->use_naming_service_ = 0;
        break;

      case 'a': // to be given only on using run_test.pl
        this->stock_name_ = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
        this->interactive_ = 0;
        break;

      case 't':
        this->threshold_value_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;


      case 'x':
        this->shutdown_ = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-f ior-file]"
                           " [-k ior]"
                           " [-x]"
                           " [-s]"
                           " [-a stock_name]"
                           " [-t threshold]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// this method uses the naming service to obtain the server object refernce.

int
Consumer_Handler::via_naming_service (void)
{
  try
    {
      // Initialization of the naming service.
      if (naming_services_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);

      CosNaming::Name notifier_ref_name (1);
      notifier_ref_name.length (1);
      notifier_ref_name[0].id = CORBA::string_dup ("Notifier");

      CORBA::Object_var notifier_obj =
        this->naming_services_client_->resolve (notifier_ref_name);

      // The CORBA::Object_var object is downcast to Notifier_var using
      // the <_narrow> method.
      this->server_ =
        Notifier::_narrow (notifier_obj.in ());


    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Consumer_Handler::via_naming_service\n");
      return -1;
    }

  return 0;
}

// Init function.
int
Consumer_Handler::init (int argc, ACE_TCHAR **argv)
{

  this->argc_ = argc;
  this->argv_ = argv;

  // Register our <Input_Handler> to handle STDIN events, which will
  // trigger the <handle_input> method to process these events.

  try
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "parse_args failed\n"),
                          -1);

      if (this->interactive_ == 1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      " Services provided:\n"
                      " * Registration <type 'r'>\n"
                      " * Unregistration <type 'u'>\n"
                      " * Quit <type 'q'>\n"));

          ACE_NEW_RETURN (consumer_input_handler_,
                          Consumer_Input_Handler (this),
                          -1);

          if (ACE_Event_Handler::register_stdin_handler
              (consumer_input_handler_,
               this->orb_->orb_core ()->reactor (),
               this->orb_->orb_core ()->thr_mgr ()) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n",
                               "register_stdin_handler"),
                              -1);

          // Register the signal event handler for ^C
          ACE_NEW_RETURN (consumer_signal_handler_,
                          Consumer_Signal_Handler (this),
                          -1);

          if (this->reactor_used ()->register_handler
              (SIGINT,
               consumer_signal_handler_) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%p\n",
                               "register_handler for SIGINT"),
                              -1);
        }
      // use the naming service.
      if (this->use_naming_service_)
        {
          if (via_naming_service () == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                                "via_naming_service failed\n"),
                              -1);
         }
      else
        {

          if (this->ior_ == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: no ior specified\n",
                               this->argv_[0]),
                              -1);

          CORBA::Object_var server_object =
            this->orb_->string_to_object (this->ior_);

          if (CORBA::is_nil (server_object.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid ior <%s>\n",
                               this->ior_),
                              -1);
          // The downcasting from CORBA::Object_var to Notifier_var is
          // done using the <_narrow> method.
          this->server_ = Notifier::_narrow (server_object.in ());
        }

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Consumer_Handler::init");
      return -1;
    }

  return 0;
}

int
Consumer_Handler::run (void)
{

  try
    {
      // Obtain and activate the RootPOA.
     CORBA::Object_var obj =
            this->orb_->resolve_initial_references ("RootPOA");

     PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in ());

     PortableServer::POAManager_var poa_manager=
       root_poa->the_POAManager ();

     poa_manager->activate ();

     ACE_NEW_RETURN (this->consumer_servant_,
                     Consumer_i (),
                     -1);
     // Set the orb in the consumer_ object.
     this->consumer_servant_->orb (this->orb_.in ());

     // Get the consumer stub (i.e consumer object) pointer.
     this->consumer_var_ =
       this->consumer_servant_->_this ();

      if (this->interactive_ == 0)
        {

          // Register with the server.
          this->server_->register_callback (this->stock_name_.c_str (),
                                            this->threshold_value_,
                                            this->consumer_var_.in ());

          // Note the registration.
          this->registered_ = 1;
          this->unregistered_ = 0;

          ACE_DEBUG ((LM_DEBUG,
                      "registeration done!\n"));
        }

      // Run the ORB.
      this->orb_->run ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Consumer_Handler::init");
      return -1;
    }

  return 0;
}

ACE_Reactor *
Consumer_Handler::reactor_used (void) const
{
  return this->orb_->orb_core ()->reactor ();
}
