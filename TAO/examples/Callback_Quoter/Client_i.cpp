// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Client_i.cpp
//
// = DESCRIPTION
//    Implementation of the Client_i class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#include "Client_i.h"
#include "ace/Read_Buffer.h"
#include "tao/ORB.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS.h"

Client_i::Client_i ()
  : ior_ (0),
    shutdown_ (0),
    stock_name_ ("Unknown"),
    threshold_value_ (0),
    server_ ()
{
}

Client_i::~Client_i (void)
{
}

// Reads the Server factory IOR from a file.

int
Client_i::read_ior (char *filename)
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
Client_i::parse_args (void)
{
  // ACE_Get_Opt get_opts (argc_, argv_, "dt:n:f:xk:xs");
  ACE_Get_Opt get_opts (argc_, argv_, "d:f:xk:xs");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
	TAO_debug_level++; //****
        break;

	/*  case 'n':  // consumer name provide on command line
        this->stock_name_ = get_opts.optarg;
        break;

      case 't':  // threshold provide on command line
        this->threshold_value_ = ACE_OS::atoi (get_opts.optarg);
        break;
	*/

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
                           " [-n stock name]"
                           " [-t threshold value]"
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


int
Client_i::via_naming_service (void)
{
  TAO_TRY
    {
      // Initialization of the naming service.
      if (naming_services_client_.init (orb_.in ()) != 0)
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize "
			   "the TAO_Naming_Client. \n"),
			  -1);
      CosNaming::Name supplier_ref_name (1);
      supplier_ref_name.length (1);
      supplier_ref_name[0].id = CORBA::string_dup ("Supplier");

      CORBA::Object_var supplier_obj =
	this->naming_services_client_->resolve (supplier_ref_name,
						TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // The CORBA::Object_var object is downcast to Supplier_var using
      // the <_narrow> method.
      this->server_ =
        Supplier::_narrow (supplier_obj.in (),
                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // I know the server , pass it to the consumer_i
      //  ACE_NEW_RETURN (this->consumer_i_,
      //		      Consumer_i (this->server_.in ()),
      //	      -1);

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client_i::via_naming_service\n");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// Init function.
int
Client_i::init (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

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

       if (this->use_naming_service_)
	 return via_naming_service ();


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

      // The downcasting from CORBA::Object_var to Supplier_var is done
      // using the <_narrow> method.
      this->server_ = Supplier::_narrow (server_object.in (),
                                         TAO_TRY_ENV);

      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client_i::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}



int
Client_i::run (void)
{
  ACE_DEBUG ((LM_DEBUG,
	      "\n============= Consumer Client! ================\n"));

  ACE_DEBUG ((LM_DEBUG,
	      " Services provided:\n "));

  ACE_DEBUG ((LM_DEBUG,
	      " 1) Registration <type \"register\">\n "));

  ACE_DEBUG ((LM_DEBUG,
	      " 2) Unregistration <type \"unregister\">\n "));

  ACE_DEBUG ((LM_DEBUG,
	      " 3) Quit <type \"quit\">\n "));



  int registered = 0;
  int unregistered = 0;
  char buf[BUFSIZ];

  TAO_TRY
    {
      for (;;)
	{
  // the string could read contains \n\0 hence using ACE_OS::read
  // which returns the no of bytes read and hence i can manipulate
  // and remove the devil from the picture i.e '\n' ! ;)
   int strlen = ACE_OS::read (ACE_STDIN, buf, sizeof buf);
   if ( buf[strlen -1] == '\n' )
     buf[strlen -1] = '\0';

       if ( ACE_OS::strcmp (buf, "register") == 0)
       {

	    CORBA::Environment TAO_TRY_ENV;
	    // I know the server , pass it to the consumer_i
	    // on creating it.
	    ACE_NEW_RETURN (this->consumer_i_,
			    Consumer_i (this->server_),
			    -1);

	    ACE_DEBUG ((LM_DEBUG,"consumer_i_ created\n"));

		// get the consumer stub (i.e consumer object) pointer.
		this->consumer_var_ =
		  this->consumer_i_->_this (TAO_TRY_ENV);
		TAO_CHECK_ENV;

		// get the stockname the consumer is interested in.
		char stockname[BUFSIZ];
		ACE_DEBUG ((LM_DEBUG,
			    "Stockname?"));

		strlen = ACE_OS::read (ACE_STDIN, stockname, sizeof stockname);

		if ( stockname[strlen -1] == '\n' )
		  stockname[strlen -1] = '\0';

		this->stock_name_ = stockname;

		// get the threshold value.
		char needed_stock_value[BUFSIZ];
		ACE_DEBUG ((LM_DEBUG,
			    "Threshold Stock value?"));

		strlen = ACE_OS::read (ACE_STDIN, needed_stock_value, sizeof needed_stock_value);


		if ( needed_stock_value[strlen -1] == '\n' )
		  needed_stock_value[strlen -1] = '\0';

		this->threshold_value_ = ACE_OS::atoi (needed_stock_value);

		// Register  with the server.
		server_->register_callback (this->stock_name_,
					    this->threshold_value_,
					    this->consumer_var_.in (),
					    TAO_TRY_ENV);

		TAO_CHECK_ENV;

		// note the registration.
		registered =1;

		ACE_DEBUG ((LM_DEBUG,
			    "registeration done!\n"));

		// Run the ORB.
		this->orb_->run ();

       }

      if ( ACE_OS::strcmp (buf, "unregister") == 0)
       {
	    if (registered == 1)
	      {

		this->server_->unregister_callback (this->consumer_var_.in ());
		ACE_DEBUG ((LM_DEBUG,
	        	    " Consumer Unregistered \n "));

		unregistered = 1;

	      }
	    else
	      {
		ACE_DEBUG ((LM_DEBUG,
			    " Invalid Operation: Consumer not Registered\n"));
	      }


       }


      if (ACE_OS::strcmp (buf, "quit") == 0)
       {
	 // get the consumer stub (i.e consumer object) pointer.
	 // this->consumer_var_ =
	 //   this->consumer_i_->_this ();


	    this->consumer_i_->shutdown (TAO_TRY_ENV);
             TAO_CHECK_ENV;

            break;
        }

     }

   }
   TAO_CATCHANY
   {
     TAO_TRY_ENV.print_exception ("Client_i::run ()");
  	return -1;
    }
    TAO_ENDTRY;

      ACE_DEBUG ((LM_DEBUG,
	    "\nreturned from run method\n"));
return 0;

}
