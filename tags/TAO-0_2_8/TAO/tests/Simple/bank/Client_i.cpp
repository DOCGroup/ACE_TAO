// $Id$

#include "Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

ACE_RCSID(Bank, Client_i, "$Id$")

// Constructor.
Client_i::Client_i (void)
  : ior_ (0),
    loop_count_ (10),
    shutdown_ (0),
    server_ ()
{
}

// Reads the Server factory ior from a file

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
  ACE_Get_Opt get_opts (argc_, argv_, "dn:f:xk:");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':  // loop count
        this->loop_count_ = (u_int) ACE_OS::atoi (get_opts.optarg);
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
      case 'x':
        this->shutdown_ = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n loopcount]"
                           " [-f ior-file]"
                           " [-k ior]"
                           " [-x]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Call the remote methods on the Account and AccountManager interface

CORBA::Float
Client_i::balance (void)
{
  // Make the RMI.

  CORBA::Float My_balance = 200.00;

  My_balance = this->server_->balance (this->env_);

  this->accountmanager_server_->open("Vishal",this->env_);

  if (this->env_.exception () != 0)
    this->env_.print_exception ("from balance");
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "balance is %f\n",
                  My_balance));
    }

return 0;
}

// Execute client example code.

int
Client_i::run (void)
{
  u_int i;

  for (i = 0; i < this->loop_count_; i++)
    {
      this->balance ();
      ACE_OS::sleep (1);
    }

  if (this->shutdown_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Operation Shutdown not defined\n"
                  ));

    }

  return 0;
}

Client_i::~Client_i (void)
{
  ACE_OS::free (this->ior_);
}


int
Client_i::init_naming_service (void)
{
  TAO_TRY
    {

      // Initialize the naming services
      if (my_name_client_.init (orb_.in (), argc_, argv_) != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize "
			   "the TAO_Naming_Client. \n"),
			  -1);

      CosNaming::Name bank_context_name (2);
      bank_context_name.length (2);
      bank_context_name[0].id =
        CORBA::string_dup ("IDL_Bank");
      bank_context_name[1].id =
        CORBA::string_dup ("Bank");

      CORBA::Object_var account_obj =
        my_name_client_->resolve (bank_context_name,
				  TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->server_ =
        Bank::Account::_narrow (account_obj.in (),
                                TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->server_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                            " could not resolve IDL_Bank:Bank in the Naming service"                         "<%s>\n"),
                            -1);


      bank_context_name[1].id =
        CORBA::string_dup ("AccountManager");

      CORBA::Object_var account_manager_obj =
        my_name_client_->resolve (bank_context_name,
				  TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->accountmanager_server_ =
        Bank::AccountManager::_narrow (account_manager_obj.in (),
                                TAO_TRY_ENV);

      TAO_CHECK_ENV;

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Bank::init_naming_service");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}


// This method uses the IOR if specified on the command line or in a file, else
// it uses the Naming Service

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


      if (this->ior_)
	{ // An ior is specified for the client through a
	  // commandline option or a file

	  CORBA::Object_var server_object =
	  this->orb_->string_to_object (this->ior_,
                                      TAO_TRY_ENV);
	  TAO_CHECK_ENV;

	  if (CORBA::is_nil (server_object.in ()))
          ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid ior <%s>\n",
                           this->ior_),
                          -1);

	  this->server_ = Bank::Account::_narrow (server_object.in (),
                                     TAO_TRY_ENV);

	  ACE_DEBUG((LM_DEBUG,"Using the IOR provided\n"));
	  TAO_CHECK_ENV;
	}
      else
	{ // No IOR specified. Use the Naming Service
	  ACE_DEBUG((LM_DEBUG,"Using the Naming Service\n"));

	  init_naming_service();
	  TAO_CHECK_ENV;
	}
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Client_i::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}
