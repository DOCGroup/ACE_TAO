// $Id$

#include "ace/Profile_Timer.h"
#include "ace/Env_Value_T.h"
#include "ace/Read_Buffer.h"
#include "client.h"
#include "eh_i.h"
#include "orbsvcs/CosNamingC.h"

ACE_RCSID(Reactor, client, "$Id$")

#define quote(x) #x

// Constructor.
NestedUpCalls_Client::NestedUpCalls_Client (void)
  : nested_up_calls_reactor_key_ (0),
    shutdown_ (0),
    call_count_ (0),
    nested_up_calls_reactor_ior_file_ (0),
    f_handle_ (ACE_INVALID_HANDLE),
    use_naming_service_ (1)
{
}

// Simple function that returns the substraction of 117 from the
// parameter.

int
NestedUpCalls_Client::func (u_int i)
{
  return i - 117;
}

// Reads the NestedUpCalls reactor ior from a file

int
NestedUpCalls_Client::read_ior (char *filename)
{
  // Open the file for reading.
  this->f_handle_ = ACE_OS::open (filename,0);

  if (this->f_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  ACE_Read_Buffer ior_buffer (this->f_handle_);
  this->nested_up_calls_reactor_key_ = ior_buffer.read ();

  if (this->nested_up_calls_reactor_key_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to allocate memory to read ior: %p\n"),
                      -1);
   return 0;
}

// Parses the command line arguments and returns an error status.

int
NestedUpCalls_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, "dn:f:k:xs");
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
      case 'k': // read the nestedupcalls IOR from the command-line.
        this->nested_up_calls_reactor_key_ =
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
                           " [-f nested_up_calls_reactor-obj-ref-key-file]"
                           " [-k nestedupcalls-obj-ref-key]"
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
NestedUpCalls_Client::run (void)
{

  TAO_TRY
    {
      // Create an EventHandler servant to hand to the other side...
      auto_ptr<EventHandler_i> eh_impl (new EventHandler_i);
      EventHandler_var eh = eh_impl->_this (TAO_TRY_ENV);

      // Now, we can invoke an operation on the remote side.
      CORBA::Long r = this->reactor_->register_handler (eh.in (), TAO_TRY_ENV);

      // We ought to have a result!
      ACE_DEBUG ((LM_DEBUG,
                  "%s: received %d as return from register_handler ()\n",
                  argv_[0], r));

      this->reactor_->set_value (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->reactor_->decrement (eh.in (), 5, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->reactor_->stop (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (this->shutdown_)
        {
          this->env_.print_exception ("server, please ACE_OS::exit");
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Nestedupcalls::run ()");
      return -1;
    }
  TAO_ENDTRY;
  return 0;
}

NestedUpCalls_Client::~NestedUpCalls_Client (void)
{
  // Free resources
  // Close the ior files
  if (this->nested_up_calls_reactor_ior_file_)
    ACE_OS::fclose (this->nested_up_calls_reactor_ior_file_);
  if (this->f_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->f_handle_);

  if (this->nested_up_calls_reactor_key_ != 0)
    ACE_OS::free (this->nested_up_calls_reactor_key_);
}

int
NestedUpCalls_Client::init_naming_service (void)
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

      CosNaming::Name nested_up_calls_reactor_name (2);
      nested_up_calls_reactor_name.length (2);
      nested_up_calls_reactor_name[0].id = CORBA::string_dup ("NestedUpCalls");
      nested_up_calls_reactor_name[1].id = CORBA::string_dup ("nested_up_calls_reactor");
      CORBA::Object_var reactor_obj =
        naming_context->resolve (nested_up_calls_reactor_name,TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->reactor_ =
        Reactor::_narrow (reactor_obj.in (),TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (this->reactor_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve nested up calls reactor in Naming service <%s>\n"),
                          -1);
   }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("NestedUpCalls::init_naming_service");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

int
NestedUpCalls_Client::init (int argc, char **argv)
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
          if (this->nested_up_calls_reactor_key_ == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "%s: no nested up calls reactor key specified\n",
                               this->argv_[0]),
                              -1);


          CORBA::Object_var reactor_object =
            this->orb_->string_to_object (this->nested_up_calls_reactor_key_,
                                          TAO_TRY_ENV);
          TAO_CHECK_ENV;

          this->reactor_ =
                Reactor::_narrow (reactor_object.in(), TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (CORBA::is_nil (this->reactor_.in ()))
            ACE_ERROR_RETURN ((LM_ERROR,
                               "invalid reactor key <%s>\n",
                               this->nested_up_calls_reactor_key_),
                              -1);
        }

      ACE_DEBUG ((LM_DEBUG, "Reactor received OK\n"));
  }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("NestedUpCalls::init");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

// This function runs the test.
int
main (int argc, char **argv)
{
  NestedUpCalls_Client nestedupcalls_client;

  ACE_DEBUG ((LM_DEBUG,
              "\n \t NestedUpCalls: client \n\n"));

  if (nestedupcalls_client.init (argc, argv) == -1)
    return 1;
  else
    return nestedupcalls_client.run ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class auto_ptr<EventHandler_i>;
template class ACE_Auto_Basic_Ptr<EventHandler_i>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate auto_ptr<EventHandler_i>
#pragma instantiate ACE_Auto_Basic_Ptr<EventHandler_i>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
