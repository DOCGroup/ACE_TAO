// $Id$

//#include "ace/Profile_Timer.h"
//#include "ace/Env_Value_T.h"
#include "ace/Read_Buffer.h"
#include "CC_client.h"
#include "orbsvcs/CosNamingC.h"

// Constructor.
CC_Client::CC_Client (void)
  : cc_factory_key_ (0),
    shutdown_ (0),
    cc_lock_set_ (CosConcurrencyControl::LockSet::_nil ()),
    cc_factory_ior_file_ (0),
    f_handle_ (ACE_INVALID_HANDLE),
    use_naming_service_ (1)
{
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
  ACE_Get_Opt get_opts (argc_, argv_, "dsf:k:x");
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
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
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-f cc_factory-obj-ref-key-file]"
                           " [-k cc-obj-ref-key]"
                           " [-x]"
                           " [-s]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Execute client example code.

int
CC_Client::run (void)
{
  CORBA::Environment _env;
  CORBA::Boolean lock_held;

  this->cc_lock_set_->lock(CosConcurrencyControl::read, _env);
  ACE_DEBUG((LM_DEBUG, "Read lock set\n"));
  lock_held = this->cc_lock_set_->try_lock(CosConcurrencyControl::read, _env);
  if(lock_held)
      ACE_DEBUG((LM_DEBUG, "Read lock not held\n"));
  else
      ACE_DEBUG((LM_DEBUG, "Read lock held\n"));
  this->cc_lock_set_->unlock(CosConcurrencyControl::read, _env);
  ACE_DEBUG((LM_DEBUG, "Read lock released\n"));
  this->cc_lock_set_->lock(CosConcurrencyControl::write, _env);
  ACE_DEBUG((LM_DEBUG, "Write lock set\n"));



  if (this->shutdown_)
    {
      // @@TAO is this needed??
    }

  return 0;
}

CC_Client::~CC_Client (void)
{
  // Free resources
  // Close the ior files
  if (this->cc_factory_ior_file_)
    ACE_OS::fclose (this->cc_factory_ior_file_);
  if (this->f_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->f_handle_);

  CORBA::release (this->cc_lock_set_);

  if (this->cc_factory_key_ != 0)
    ACE_OS::free (this->cc_factory_key_);
}

int
CC_Client::init_naming_service (void)
{
  TAO_TRY
    {
      CORBA::Object_var naming_obj =
        this->orb_->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to resolve the Name Service.\n"),
                          -1);
      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (),
                                           TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CosNaming::Name cc_factory_name (2);
      cc_factory_name.length (2);
      cc_factory_name[0].id = CORBA::string_dup ("CosConcurrency");
      cc_factory_name[1].id = CORBA::string_dup ("LockSetFactory");
      CORBA::Object_var factory_obj =
        naming_context->resolve (cc_factory_name,TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->factory_ =
        CosConcurrencyControl::LockSetFactory::_narrow
          (factory_obj.in (),TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->factory_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve lock set factory in Naming service <%s>\n"),
                          -1);
   }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("CC_Client::init_naming_service");
      return -1;
    }
  TAO_ENDTRY;

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

          this->factory_ =
            CosConcurrencyControl::LockSetFactory::_narrow
              (factory_object.in(), TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (CORBA::is_nil (this->factory_.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid factory key <%s>\n",
                               this->cc_factory_key_),
                              -1);
        }

      ACE_DEBUG ((LM_DEBUG, "Factory received OK\n"));

      // Now create the lock set and return an obj ref corresponding
      // to the key.
      this->cc_lock_set_ =
        this->factory_->create (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->cc_lock_set_))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "null lock set objref returned by factory\n"),
                          -1);
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
