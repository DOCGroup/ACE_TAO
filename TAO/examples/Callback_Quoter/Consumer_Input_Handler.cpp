// $Id$
// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Consumer_Input_Handler.cpp
//
// = DESCRIPTION
//    Implementation of the Consumer_Input_Handler class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#include "Consumer_Input_Handler.h"
#include "ace/Read_Buffer.h"
#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS.h"

Consumer_Input_Handler::Consumer_Input_Handler ()
  : ior_ (0),
    shutdown_ (0),
    stock_name_ ("Unknown"),
    threshold_value_ (0),
    server_ (),
    registered_(0),
    unregistered_(0)
{
}

Consumer_Input_Handler::~Consumer_Input_Handler (void)
{
  // Make sure to cleanup the STDIN handler.

  if (ACE_Event_Handler::remove_stdin_handler
      (TAO_ORB_Core_instance ()->reactor (),
       TAO_ORB_Core_instance ()->thr_mgr ()) == -1)
     ACE_ERROR ((LM_ERROR,
       	       "%p\n",
       	       "remove_stdin_handler"));

}

// Reads the Server factory IOR from a file.

int
Consumer_Input_Handler::read_ior (char *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
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

  return 0;
}

// Parses the command line arguments and returns an error status.

int
Consumer_Input_Handler::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "d:f:xk:xs");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
	TAO_debug_level++; //****
        break;

      case 'k':  // ior provide on command line
        this->ior_ = ACE_OS::strdup (get_opts.optarg);
        break;

      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);
        break;

      case 's': // don't use the naming service
	this->use_naming_service_ = 0;
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
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// this method uses the naming service to obtain the server object refernce.

int
Consumer_Input_Handler::via_naming_service (void)
{
  TAO_TRY
    {
      // Initialization of the naming service.
      if (naming_services_client_.init (orb_.in ()) != 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize "
			   "the TAO_Naming_Client. \n"),
			  -1);

      CosNaming::Name notifier_ref_name (1);
      notifier_ref_name.length (1);
      notifier_ref_name[0].id = CORBA::string_dup ("Notifier");

      CORBA::Object_var notifier_obj =
	this->naming_services_client_->resolve (notifier_ref_name,
						TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The CORBA::Object_var object is downcast to Notifier_var using
      // the <_narrow> method.
      this->server_ =
        Notifier::_narrow (notifier_obj.in (),
                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Consumer_Input_Handler::via_naming_service\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// Init function.
int
Consumer_Input_Handler::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

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

  TAO_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    0,
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "parse_args failed\n"),
			   -1);

      // use the naming service.
       if (this->use_naming_service_)
	 return  via_naming_service ();


       if (this->ior_ == 0)
         ACE_ERROR_RETURN ((LM_ERROR,
                            "%s: no ior specified\n",
                            this->argv_[0]),
                           -1);

      CORBA::Object_var server_object =
        this->orb_->string_to_object (this->ior_,
                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (server_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid ior <%s>\n",
                           this->ior_),
                          -1);

      // The downcasting from CORBA::Object_var to Notifier_var is done
      // using the <_narrow> method.
      this->server_ = Notifier::_narrow (server_object.in (),
                                         TAO_TRY_ENV);

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Consumer_Input_Handler::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}


int
Consumer_Input_Handler::run (void)
{
  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      ACE_NEW_RETURN (this->consumer_servant_,
		      Consumer_i (),
		      -1);
      // Set the orb in the consumer_ object.
      this->consumer_servant_->orb (this->orb_.in ());

      ACE_DEBUG ((LM_DEBUG,
		  " Services provided:\n "
		  " * Registration <type 'r'>\n "
		  " * Unregistration <type 'u'>\n "
		  " * Quit <type 'q'>\n "));

      // Get the consumer stub (i.e consumer object) pointer.
      this->consumer_var_ =
	this->consumer_servant_->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Run the ORB.
      this->orb_->run ();
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Consumer_Input_Handler::run ()");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

int
Consumer_Input_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  TAO_TRY
    {
      // The string could read contains \n\0 hence using ACE_OS::read
      // which returns the no of bytes read and hence i can manipulate
      // and remove the devil from the picture i.e '\n' ! ;)

      ssize_t strlen = ACE_OS::read (ACE_STDIN,
				     buf,
				     sizeof buf);
      if (buf[strlen -1] == '\n')
	buf[strlen -1] = '\0';

      ACE_DEBUG ((LM_DEBUG,
		  "%s",
		  buf));

      // @@ Whenever you've got big giant case statements break them
      // into separate helper functions.
      switch (tolower (buf[0]))
	{
	case 'r':
	  {
	    // Get the stockname the consumer is interested in.
	    char stockname[BUFSIZ];

	    ACE_DEBUG ((LM_DEBUG,
			"Stockname?"));

	    strlen = ACE_OS::read (ACE_STDIN,
				   stockname,
				   sizeof stockname);

	    if (stockname[strlen -1] == '\n')
	      stockname[strlen -1] = '\0';

	    this->stock_name_ = stockname;

	    // Get the threshold value.
	    char needed_stock_value[BUFSIZ];
	    ACE_DEBUG ((LM_DEBUG,
			"Threshold Stock value?"));

	    strlen = ACE_OS::read (ACE_STDIN,
				   needed_stock_value,
				   sizeof needed_stock_value);

	    if (needed_stock_value[strlen -1] == '\n')
	      needed_stock_value[strlen -1] = '\0';

	    this->threshold_value_ =
	      ACE_OS::atoi (needed_stock_value);

	    // Register with the server.
	    server_->register_callback (this->stock_name_,
					this->threshold_value_,
					this->consumer_var_.in (),
					TAO_TRY_ENV);
	    TAO_CHECK_ENV;

	    // Note the registration.
	    registered_ = 1;
	    ACE_DEBUG ((LM_DEBUG,
			"registeration done!\n"));
	    break;
	  }
	case 'u':
	  {
	    // Only if the consumer is registered can the
	    // unregistration take place.

	    if (registered_ == 1)
	      {
		this->server_->unregister_callback (this->consumer_var_.in ());
		ACE_DEBUG ((LM_DEBUG,
	        	    " Consumer Unregistered \n "));
		unregistered_ = 1;
	      }
	    else
	      ACE_DEBUG ((LM_DEBUG,
			  " Invalid Operation: Consumer not Registered\n"));
	    break;
	  }
        case 'q':
	  {
	    // Only if the consumer is registered and wants to shut
	    // down, its necessary to unregister and then shutdown.

	    if (unregistered_ != 1 && registered_ == 1)
	      {
		this->server_->unregister_callback (this->consumer_var_.in ());
		ACE_DEBUG ((LM_DEBUG,
	        	    " Consumer Unregistered \n "));
	      }
	    this->consumer_servant_->shutdown (TAO_TRY_ENV);
	    TAO_CHECK_ENV;
            break;
	  }
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Input_Handler::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
