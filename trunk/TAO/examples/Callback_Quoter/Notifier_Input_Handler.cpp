// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Notifier_Input_Handler.cpp
//
// = DESCRIPTION
//    Implementation of the callback quoter Notifier_Input_Handler class.
//
// = AUTHOR
//   Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#include "Notifier_Input_Handler.h"
#include "ace/Get_Opt.h"

// Constructor.

Notifier_Input_Handler::Notifier_Input_Handler (void)
  : ior_output_file_ (0),
    argc_ (0),
    argv_ (0),
    using_naming_service_ (1)
{
  // no-op.
}

// Destructor.

Notifier_Input_Handler::~Notifier_Input_Handler (void)
{
   // Make sure to cleanup the STDIN handler.

  if (ACE_Event_Handler::remove_stdin_handler
      (TAO_ORB_Core_instance ()->reactor (),
       TAO_ORB_Core_instance ()->thr_mgr ()) == -1)
     ACE_ERROR ((LM_ERROR,
       	       "%p\n",
       	       "remove_stdin_handler"));

}

// The naming service is initialized and the naming context as well as
// the object name is bound to the naming server.

int
Notifier_Input_Handler::init_naming_service (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      CORBA::ORB_var orb = this->orb_manager_.orb ();

      PortableServer::POA_var child_poa
	= this->orb_manager_.child_poa ();

      int return_val =
	this->naming_server_.init (orb.in (),
				   child_poa.in ());
      if (return_val == -1)
	ACE_ERROR_RETURN ((LM_ERROR,
			   "Failed to initialize TAO_Naming_Server\n"),
			  -1);

      // Register the object implementation with the POA.
      Notifier_var notifier_obj = this->notifier_i_._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Name the object.
      CosNaming::Name notifier_obj_name (1);
      notifier_obj_name.length (1);
      notifier_obj_name[0].id = CORBA::string_dup ("Notifier");

      ACE_TRY_CHECK;

      // Now, attach the object name to the context.
      this->naming_server_->bind (notifier_obj_name,
				  notifier_obj.in (),
				  ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("Notifier_Input_Handler::init_naming_service\n");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

// Parse the command-line arguments and set options.
int
Notifier_Input_Handler::parse_args (void)
{
  ACE_Get_Opt get_opts (this->argc_, this->argv_, "df:s ");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;  ///*****
        break;

      case 'f':  // output the IOR toi a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.optarg, "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.optarg),
                            -1);
        break;

      case 's': // don't use the naming service
	this->using_naming_service_ = 0;
        break;

      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-f] <ior_output_file>"
			   " [-s]"
                           "\n",
                           argv_ [0]),
                          1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Initialize the server.

int
Notifier_Input_Handler::init (int argc,
                              char *argv[],
                              CORBA::Environment &ACE_TRY_ENV)
{

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

  // Call the init of <TAO_ORB_Manager> to initialize the ORB and
  // create the child poa under the root POA.

  if (this->orb_manager_.init_child_poa (argc,
					 argv,
					 "child_poa",
					 ACE_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "init_child_poa"),
                      -1);
  ACE_CHECK_RETURN (-1);

  this->argc_ = argc;
  this->argv_ = argv;

  int retval = this->parse_args ();

  if (retval != 0)
    return retval;

  CORBA::ORB_var orb = this->orb_manager_.orb ();

  // Stash our ORB pointer for later reference.
  this->notifier_i_.orb (orb.in ());

  // Activate the servant in the POA.
  CORBA::String_var str  =
    this->orb_manager_.activate_under_child_poa ("Notifier",
						 &this->notifier_i_,
						 ACE_TRY_ENV);
  ACE_DEBUG ((LM_DEBUG,
              "The IOR is: <%s>\n",
              str.in ()));

  if (this->ior_output_file_)
    {
      ACE_OS::fprintf (this->ior_output_file_,
                       "%s",
                       str.in ());
      ACE_OS::fclose (this->ior_output_file_);
    }

  if (this->using_naming_service_)
    return this->init_naming_service (ACE_TRY_ENV);

  return 0;
}

int
Notifier_Input_Handler::run (CORBA::Environment &ACE_TRY_ENV)
{
  // Run the main event loop for the ORB.


  ACE_DEBUG ((LM_DEBUG,
	      " Type \"q\" to quit \n "));

  if (this->orb_manager_.run (ACE_TRY_ENV) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Notifier_Input_Handler::run"),
                      -1);
  return 0;
}

int
Notifier_Input_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  ACE_TRY_NEW_ENV
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

      if (tolower(buf[0]) == 'q')
	{
          // @@ Please remove this call if it's not used.
	  // (this->notifier_i_.consumer_map_).close();
	  this->notifier_i_.shutdown (ACE_TRY_ENV);
          ACE_TRY_CHECK;
	}
    }
   ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("Input_Handler::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
