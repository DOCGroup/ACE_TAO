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

Client_i::Client_i ()
  : ior_ (0),
    shutdown_ (0),
    server_ (),
    use_naming_service_ (1),
    stock_name_ ("Unknown"),
    threshold_value_ (0)
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
  ACE_Get_Opt get_opts (argc_, argv_, "dt:n:f:xk:xs");

  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
	TAO_debug_level++; //****
        break;

      case 'n':  // consumer name provide on command line
        this->stock_name_ = get_opts.optarg;
        break;

      case 't':  // threshold provide on command line
        this->threshold_value_ = ACE_OS::atoi (get_opts.optarg);
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
        return -1;

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
	      "\n============= Consumer Client! =================\n"));

  CORBA::Environment TAO_TRY_ENV;

  TAO_TRY
    {
      this->consumer_var_ =
	this->consumer_i_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Register  with the server.
      server_->register_callback (this->stock_name_,
				  this->threshold_value_,
				  this->consumer_var_.in (),
				  TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Run the ORB.
      this->orb_->run ();
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client_i::run ()");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

