// $Id$

#include "ace/Read_Buffer.h"
#include "CC_client.h"

// Constructor.
CC_Client::CC_Client (void)
  : cc_factory_key_ (0),
    shutdown_ (0),
    cc_factory_ior_file_ (0),
    f_handle_ (ACE_INVALID_HANDLE),
    use_naming_service_ (1),
    run_basic_tests_ (0),
    naming_service_ (0)
{
}

CC_Client::~CC_Client (void)
{
  // Free resources
  // Close the ior files
  if (this->cc_factory_ior_file_)
    ACE_OS::fclose (this->cc_factory_ior_file_);
  if (this->f_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->f_handle_);

  if (this->cc_factory_key_ != 0)
    ACE_OS::free (this->cc_factory_key_);
  if(naming_service_!=0)
    delete naming_service_;
}

// Reads the lock set factory ior from a file

int
CC_Client::read_ior (char *filename)
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
  ACE_Get_Opt get_opts (argc_, argv_, "dsf:k:xbh");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'b':  // debug flag
        run_basic_tests_ = 1;
        break;
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'f': // read the IOR from the file.
        result = this->read_ior (get_opts.optarg);
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.optarg),
                            -1);
            break;
      case 'k': // read the cubit IOR from the command-line.
        this->cc_factory_key_ =
          ACE_OS::strdup (get_opts.optarg);
        break;
      case 'x':
        this->shutdown_ = 1;
        break;
      case 's': // Don't use the TAO Naming Service.
        this->use_naming_service_ = 0;
        break;
      case 'h':
      default:
        print_usage();
        ACE_ERROR_RETURN((LM_ERROR, ""), -1);
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
  int success = 0;
  // Did test succeed?

  if(run_basic_tests_) 
    {
      success = run_basic_tests();
      tests_run = 1;
    }

  // Other tests go here
  // if(other_test_flag && success==CC_SUCCESS) ...

  if (this->shutdown_)
    {
      // @@TAO is this needed??
    }

  if (tests_run==0)
    {
      print_usage();
      ACE_ERROR_RETURN((LM_ERROR, "No tests given\n"), -1);
    }

  return 0;
}


// This function runs basic tests concerned with only one lock set
int
CC_Client::run_basic_tests(void)
{
  Test_Single_Lock_With_Mode t1(naming_service_,
                                CosConcurrencyControl::read);

  return t1.run();
}

int 
CC_Client::run_extended_tests(char lock_set_name)
{
  Test_Against_Other_LockSet t1(naming_service_,
                                "LockSet_1");

  return t1.run();
}

void
CC_Client::print_usage(void)
{
  ACE_ERROR ((LM_ERROR,
              "usage:  %s"
              " [-b]"
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
  naming_service_ = new CC_naming_service(this->orb_);

  return 0;
}

int
CC_Client::init (int argc, char **argv)
{
  int naming_result;
  this->argc_ = argc;
  this->argv_ = argv;

  TAO_TRY
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_,
                                    this->argv_,
                                    "internet",
                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

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
            this->orb_->string_to_object (this->cc_factory_key_,
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;

          /* The test cannot currently run without the naming service
                      this->factory_ =
            CosConcurrencyControl::LockSetFactory::_narrow
              (factory_object.in(), TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (CORBA::is_nil (this->factory_.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid factory key <%s>\n",
                               this->cc_factory_key_),
                              -1);*/
        }

      ACE_DEBUG ((LM_DEBUG, "Factory received OK\n"));

    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_Client::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// This function runs the test.

int
main (int argc, char **argv)
{
  CC_Client cc_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t CosConcurrencyControl: client \n\n"));

  if (cc_client.init (argc, argv) == -1)
    {
        ACE_DEBUG((LM_DEBUG, "Did not initialize correctly\n"));
        return 1;
    }
  else
    {
      ACE_DEBUG((LM_DEBUG, "Running the test\n"));
      return cc_client.run ();
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Env_Value<unsigned long>;
template class ACE_Env_Value<unsigned short>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Env_Value<unsigned long>
#pragma instantiate ACE_Env_Value<unsigned short>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
