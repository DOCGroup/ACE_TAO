// $Id$

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "Client_i.h"

ACE_RCSID(Bank, Client_i, "$Id$")

// Constructor.

Client_i::Client_i (void)
  : ior_ (0),
    loop_count_ (10),
    shutdown_ (0),
    server1_ (),
    server2_ ()
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
                       "[CLIENT] Process/Thread Id : (%P/%t) Unable to open %s for writing: %p\n",
                       filename),
                      -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "[CLIENT] Process/Thread Id : (%P/%t) Unable to read ior: %p\n"),
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
  ACE_Get_Opt get_opts (argc_, argv_, "dn:b:y:z:f:xk:");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    // ACE_DEBUG((LM_DEBUG,"bal =1%c",c
    //             ));
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':  // loop count
        this->loop_count_ = (u_int) ACE_OS::atoi (get_opts.optarg);
        break;
      case 'b':  // initial balance
        this->initial_balance_ = (float) ACE_OS::atoi (get_opts.optarg);
      break;
      case 'y': // Name of one account holder.
        this->account_holder_name1_ = ACE_OS::strdup (get_opts.optarg);
        break;
      case 'z': // Name of another account holder.
        this->account_holder_name2_ = ACE_OS::strdup (get_opts.optarg);
        break;
      case 'k':  // ior provide on command line
        this->ior_ = ACE_OS::strdup (get_opts.optarg);
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "[CLIENT] Process/Thread Id : (%P/%t) Unable to read ior from %s : %p\n",
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
                           " [-b Initial Balance]"
                           " [-y First Account Holder's Name]"
                           " [-z Second Account Holder's Name]"
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

void
Client_i::deposit (Bank::Account_ptr server,
                   CORBA::Float deposit_amount,
                   CORBA::Environment &)
{
  server->deposit (deposit_amount,
                   this->env_);
}

void
Client_i::withdraw (Bank::Account_ptr server,
                    CORBA::Float withdrawl_amount)
{
  TAO_TRY
    {
      server->withdraw (withdrawl_amount,
                        TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      Bank::Account::Overdraft_ptr except =
        Bank::Account::Overdraft::_narrow
        (TAO_TRY_ENV.exception ());

      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t): %s",
                  except->reason.in ()));
    }
  TAO_ENDTRY;
}

Bank::Account_ptr
Client_i::open (const char *name,
                CORBA::Float initial_balance,
                CORBA::Environment &env)
{
  return this->accountmanager_server_->open (name,
                                             initial_balance,
                                             env);
}

void
Client_i::close (Bank::Account_ptr account,
                 CORBA::Environment &env)
{
  this->accountmanager_server_->close (account,
                                       env);
}


// This method tests if opening an account with the same name returns
// the same IOR.

void
Client_i::test_for_same_name (CORBA::Environment &)
{
  this->server1_ = this->open (this->account_holder_name1_,
                               this->initial_balance_,
                               this->env_);
  this->server2_ = this->open (this->account_holder_name1_,
                               this->initial_balance_,
                               this->env_);
  ACE_ASSERT (server1_->_is_equivalent ((CORBA::Object *) server2_.in ()) != 0);

  this->close (server1_.in (),
               this->env_);
  this->close (server2_.in (),
               this->env_);
}

// This method tests if opening an account with different names
// returns a different IOR.
void
Client_i::test_for_different_name (CORBA::Environment &)
{
  this->server1_ = this->open (this->account_holder_name1_,
                               this->initial_balance_,
                               this->env_);
  this->server2_ = this->open (this->account_holder_name2_,
                               this->initial_balance_,
                               this->env_);

  ACE_ASSERT (server1_->_is_equivalent ((CORBA::Object *)server2_.in ()) == 0);

  this->close (server1_.in (),
               this->env_);
  this->close (server2_.in (),
               this->env_);
}

// This method tests the Overdraft exception.

void
Client_i::test_for_overdraft (CORBA::Environment &)
{
  this->server1_ = this->open (this->account_holder_name1_,
                               this->initial_balance_,
                               this->env_);
  this->deposit (server1_.in (),
                 100.00,
                 this->env_);
  this->withdraw (server1_.in (),
                  server1_->balance(this->env_) + 20);
  this->close (server1_.in (),
               this->env_);
}


CORBA::Float
Client_i::check_accounts (void)
{
  TAO_TRY
    {
      ACE_DEBUG((LM_DEBUG,
                 "\n[CLIENT] Process/Thread Id : (%P/%t):Test for Accounts with same name\n"));
      this->test_for_same_name (this->env_);
      TAO_CHECK_ENV;

      ACE_DEBUG((LM_DEBUG,
                 "\n[CLIENT] Process/Thread Id : (%P/%t):Test for Accounts with different name\n"));
      this->test_for_different_name (this->env_);
      TAO_CHECK_ENV;

      ACE_DEBUG((LM_DEBUG,
                 "\n[CLIENT] Process/Thread Id : (%P/%t):Test for Overdraft Exception\n"));
      this->test_for_overdraft (this->env_);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      if (this->env_.exception () != 0)
        this->env_.print_exception ("From Client_i::check_accounts()");
    }
  TAO_ENDTRY;

  return 0;
}

// Execute client example code.

int
Client_i::run (void)
{
  this->check_accounts ();

  TAO_TRY
    {
      if (this->shutdown_)
        this->accountmanager_server_->shutdown (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "[CLIENT] Process/Thread Id : (%P/%t) Unable to shut down the server\n"));
    }
  TAO_ENDTRY;

return 0;

}

Client_i::~Client_i (void)
{
  ACE_OS::free (this->ior_);
}

int
Client_i::obtain_initial_references (void)
{
  TAO_TRY
    {
      // Initialize the naming services.
      if (my_name_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "[CLIENT] Process/Thread Id : (%P/%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      CosNaming::Name account_manager_name (1);
      account_manager_name.length (1);
      account_manager_name[0].id =
      CORBA::string_dup ("AccountManager");

      CORBA::Object_var account_manager_obj =
        my_name_client_->resolve (account_manager_name,
                                  TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->accountmanager_server_ =
        Bank::AccountManager::_narrow (account_manager_obj.in (),
                                       TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Bank::obtain_initial_references");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// This method uses the IOR if specified on the command line or in a
// file, else it uses the Naming Service

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
        {
          // An ior is specified for the client through a commandline
          // option or a file.

          CORBA::Object_var server_object =
          this->orb_->string_to_object (this->ior_,
                                        TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (CORBA::is_nil (server_object.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid ior <%s>\n",
                               this->ior_),
                              -1);

          this->accountmanager_server_ =
            Bank::AccountManager::_narrow (server_object.in (),
                                           TAO_TRY_ENV);

          ACE_DEBUG ((LM_DEBUG,
                      "[CLIENT] Process/Thread Id : (%P/%t) Using the IOR provided\n"));
          TAO_CHECK_ENV;
        }
      else
        { // No IOR specified. Use the Naming Service
          ACE_DEBUG((LM_DEBUG,
                     "[CLIENT] Process/Thread Id : (%P/%t) Using the Naming Service\n"));

          this->obtain_initial_references ();
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
