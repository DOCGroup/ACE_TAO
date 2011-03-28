
//=============================================================================
/**
 *  @file    CC_client.h
 *
 *  $Id$
 *
 *    This is the test class for the concurrency service. The class
 *    implements a client to the concurrency service.
 *    This file contains the main function for the test.
 *
 *
 *  @author   Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#include "CC_client.h"

#include "tao/debug.h"

#include "ace/Read_Buffer.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_fcntl.h"



// Constructor.
CC_Client::CC_Client (void)
  : naming_service_ (0),
    cc_factory_ior_file_ (0),
    cc_factory_key_ (0),
    f_handle_ (ACE_INVALID_HANDLE),
    shutdown_ (0),
    use_naming_service_ (1),
    run_basic_tests_ (0),
    run_extended_tests_ (0),
    use_script_file_ (0),
    script_file_ (0)
{
}

CC_Client::~CC_Client (void)
{
  // Free resources and close the ior files.
  if (this->cc_factory_ior_file_)
    ACE_OS::fclose (this->cc_factory_ior_file_);

  if (this->f_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->f_handle_);

  if (this->cc_factory_key_ != 0)
    ACE_OS::free (this->cc_factory_key_);

  if (naming_service_!=0)
    delete naming_service_;
}

// Reads the lock set factory ior from a file

int
CC_Client::read_ior (ACE_TCHAR *filename)
{
  // Open the file for reading.
  this->f_handle_ = ACE_OS::open (filename,0);

  if (this->f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (this->f_handle_);
  this->cc_factory_key_ = ior_buffer.read ();

  if (this->cc_factory_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);
   return 0;
}

// Parses the command line arguments and returns an error status.

int
CC_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("dc:sf:k:xbhe:"));
  int c;
  int result;

  if(argc_==1) // No arguments given on command line
    {
      print_usage();
      return -1;
    }

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':  // debug flag
        this->run_basic_tests_ = 1;
        break;
      case 'c':
        this->use_script_file_ = 1;
        this->script_file_ = ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'e':  // debug flag
        run_extended_tests_ = 1;
        this->extended_tests_params_ = ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.opt_arg ());
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);
            break;
      case 'k': // read the cubit IOR from the command-line.
        this->cc_factory_key_ =
          ACE_OS::strdup (ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ()));
        break;
      case 'x':
        this->shutdown_ = 1;
        break;
      case 's': // Don't use the TAO Naming Service.
        this->use_naming_service_ = 0;
        break;
      case 'h':
      default:
        print_usage ();
        ACE_ERROR_RETURN ((LM_ERROR, ""), -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Execute client example code.

int
CC_Client::run (void)
{
  int tests_run = 0;
  // Tells whether any tests have been run

  int success = CC_SUCCESS;
  // Did test succeed?

  if (this->run_basic_tests_ && success == CC_SUCCESS)
    {
      success = run_basic_tests ();
      if(success==CC_FAIL)
        ACE_DEBUG((LM_DEBUG, "Basic tests did not succeed\n"));
      tests_run = 1;
    }

  if (this->run_extended_tests_ && success == CC_SUCCESS)
    {
      success = run_extended_tests (this->extended_tests_params_);
      if(success==CC_FAIL)
        ACE_DEBUG((LM_DEBUG, "Extended tests did not succeed\n"));
      tests_run = 1;
    }

  if(this->use_script_file_ && success == CC_SUCCESS)
    {
      cmdlist = new CC_CommandList();
      FILE *f;

      // Open the command file for parsing if the filename!=stdin
      if(ACE_OS::strcmp(this->script_file_, ACE_TEXT("stdin"))!=0)
        {
          f = ACE_OS::fopen(this->script_file_, "r");
          if(f==0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to open %s\n",
                               this->script_file_),
                              -1);
          ace_cc_yyin = f;
        }
      ace_cc_yyparse();
    }

  // Other tests go here
  // if (other_test_flag && success == CC_SUCCESS) ...

  if (this->shutdown_)
    // @@TAO is this needed??

  if (tests_run == 0)
    {
      print_usage ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         "No tests given\n"),
                        -1);
    }

  return success;
}

// This function runs basic tests concerned with only one lock set

int
CC_Client::run_basic_tests (void)
{
  Test_Single_Lock_With_Mode t1 (naming_service_,
                                 CosConcurrencyControl::read);
  Test_Single_Lock_With_Mode t2 (naming_service_,
                                 CosConcurrencyControl::write);
  Test_Single_Lock_With_Mode t3 (naming_service_,
                                 CosConcurrencyControl::upgrade);
  Test_Single_Lock_With_Mode t4 (naming_service_,
                                 CosConcurrencyControl::intention_read);
  Test_Single_Lock_With_Mode t5 (naming_service_,
                                 CosConcurrencyControl::intention_write);
  // This test should be run for several different lock mode, but
  // since we do not support
  Test_Release_Not_Held_Lock t6 (naming_service_,
                                 CosConcurrencyControl::read);
  if (t1.run () == CC_SUCCESS &&
      t2.run () == CC_SUCCESS &&
      t3.run () == CC_SUCCESS &&
      t4.run () == CC_SUCCESS &&
      t5.run () == CC_SUCCESS &&
      t6.run () == CC_SUCCESS )
    return CC_SUCCESS;
  else
    return CC_FAIL;
}

int
CC_Client::check_extended_test_params(ACE_TCHAR *params)
{
  // Format (regexp): [0-9]+;.*;.*
  int no_of_params = 0;
  ACE_TCHAR *cp = params; // pointer to walk along the string
  enum {TAO_START, TAO_NUMBER, TAO_ARG, TAO_ERROR} state = TAO_START;

  while(*cp!='\0')
    {
      switch(state)
        {
        case TAO_START:
          if(ACE_OS::ace_isdigit(*cp))
            state = TAO_NUMBER;
          else
            state = TAO_ERROR;
          break;

        case TAO_NUMBER:
          if((*cp)==';')
            {
              state = TAO_ARG;
              no_of_params++;
            }
          else
            if(!ACE_OS::ace_isdigit(*cp))
              state = TAO_ERROR;
          break;

        case TAO_ARG:
          if((*cp)==';')
            {
              no_of_params++;
            }
          break;

        case TAO_ERROR:
          return -1;
          // break;

        default:
          ACE_ERROR_RETURN((LM_ERROR,
                            "CC_Client::check_extended_test_params\n"), -1);
        }
      cp++;
    }
  if (state==TAO_ERROR) // there was only one character given and it was wrong
    return -1;
  else
    return no_of_params;
}

int
CC_Client::run_extended_tests (ACE_TCHAR *params)
{
  int success = CC_FAIL;
  int no_of_args = 0;

  ACE_DEBUG ((LM_DEBUG,
              "Params: %s\n",
              params));

  no_of_args = check_extended_test_params(params);
  if(no_of_args==-1)
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        "Error in parameter string (%s). Format: '<test#>;<arg1>;<arg2>'\n", params), CC_FAIL);
    }

  ACE_DEBUG((LM_DEBUG, "Number of arguments: %i\n", no_of_args));

  char *cmd  = ACE_OS::strtok (ACE_TEXT_ALWAYS_CHAR(params), ";");
  char *arg1 = ACE_OS::strtok (0, ";");
  //  char *arg2 = ACE_OS::strtok (0, ";");

  // A possible scenario using test 1,2, and 3 Create and lock the
  // lock set with the name 'Name'
  // ./CC_client -e '1;Name'
  // Try to lock the same lock set. The process will hang
  // ./CC_client -e '2:Name'
  // Unlocks the lock set. Now test 2 will continue.
  // ./CC_client -e '3:Name'

  if (ACE_OS::strcmp (cmd, "1") == 0)
    {
      Test_Setup_LockSet t1 (naming_service_, arg1);
      success = t1.run ();
    }

  if (ACE_OS::strcmp (cmd, "2") == 0)
    {
      Test_Use_Already_Created_LockSet t2 (naming_service_, arg1);
      success = t2.run ();
    }

  if (ACE_OS::strcmp (cmd, "3") == 0)
    {
      Test_Unlock_Already_Created_LockSet t3 (naming_service_, arg1);
      success = t3.run ();
    }

  return success;
}

void
CC_Client::print_usage (void)
{
  ACE_ERROR ((LM_ERROR,
              "usage:  %s"
              " [-b]"
              " [-c] cc-test-script"
              " [-d]"
              " [-f cc_factory-obj-ref-key-file]"
              " [-k cc-obj-ref-key]"
              " [-x]"
              " [-s]"
              "\n",
              this->argv_ [0]));
}

int
CC_Client::init_naming_service (void)
{
  try
    {
      ACE_NEW_RETURN (naming_service_,
                      CC_naming_service,
                      -1);

      this->naming_service_->Init (this->orb_);
    }
  catch (const CORBA::Exception&)
    {
      return -1;
    }
  return 0;
}

int
CC_Client::init (int argc, ACE_TCHAR **argv)
{
  int naming_result;
  this->argc_ = argc;
  this->argv_ = argv;

  try
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet");

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      if (this->use_naming_service_)
        {
          naming_result = this->init_naming_service ();
          if (naming_result < 0)
            return naming_result;
        }
      else
        {
          if (this->cc_factory_key_ == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: no lock set factory key specified\n",
                               this->argv_[0]),
                              -1);


          CORBA::Object_var factory_object =
            this->orb_->string_to_object (this->cc_factory_key_);

#if 0
          // The test cannot currently run without the naming service.
          this->factory_ =
            CosConcurrencyControl::LockSetFactory::_narrow
            (factory_object.in ());

          if (CORBA::is_nil (this->factory_.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid factory key <%s>\n",
                               this->cc_factory_key_),
                              -1);
#endif /* 0 */
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Factory received OK\n"));

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("CC_Client::init");
      return -1;
    }

  return 0;
}

// This function runs the test.

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  CC_Client cc_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t CosConcurrencyControl: client \n\n"));

  if (cc_client.init (argc, argv) == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Did not initialize correctly\n"));
      return 1;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Running the test\n"));
      return cc_client.run ();
    }
}
