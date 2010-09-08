// $Id$

#include "PP_Test_Client.h"

#include "tao/Timeprobe.h"
#include "tao/TAO_Internal.h"
#include "tao/debug.h"

#include "ace/Read_Buffer.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"

#if defined (ACE_ENABLE_TIMEPROBES)

  static const char *PP_Test_Client_Timeprobe_Description[] =
{
  "PP_Test_Client::send_oneway - start",
  "PP_Test_Client::send_oneway - end",

  "PP_Test_Client::send_void - start",
  "PP_Test_Client::send_void - end",

  "PP_Test_Client::make_pluggable - start",
  "PP_Test_Client::make_pluggable - end",

  "PP_Test_Client::server_shutdown - start",
  "PP_Test_Client::server_shutdown - end"
};

enum
{
  // Timeprobe description table start key
  PP_TEST_CLIENT_SEND_ONEWAY_START = 10000,
  PP_TEST_CLIENT_SEND_ONEWAY_END,

  PP_TEST_CLIENT_SEND_VOID_START,
  PP_TEST_CLIENT_SEND_VOID_END,

  PP_TEST_CLIENT_MAKE_PLUGGABLE_START,
  PP_TEST_CLIENT_MAKE_PLUGGABLE_END,

  PP_TEST_CLIENT_SERVER_SHUTDOWN_START,
  PP_TEST_CLIENT_SERVER_SHUTDOWN_END
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (PP_Test_Client_Timeprobe_Description,
                                  PP_TEST_CLIENT_SEND_ONEWAY_START);

#endif /* ACE_ENABLE_TIMEPROBES */

// Constructor.
PP_Test_Client::PP_Test_Client (int shutdown)
  : orb_ (0),
    factory_key_ (0),
    loop_count_ (1),
    shutdown_ (shutdown),
    objref_ (Pluggable_Test::_nil ()),
    call_count_ (0),
    error_count_ (0),
    factory_ior_file_ (0),
    f_handle_ (ACE_INVALID_HANDLE),
    only_void_ (0),
    only_oneway_ (0)
{
}

// Reads the Cubit factory ior from a file

int
PP_Test_Client::read_ior (ACE_TCHAR *filename)
{
  // Open the file for reading.
  this->f_handle_ = ACE_OS::open (filename,0);

  if (this->f_handle_ == ACE_INVALID_HANDLE)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unable to open %s for writing: %p\n",
                         filename),
                        -1);
    }

  ACE_Read_Buffer ior_buffer (this->f_handle_);

  char *data = ior_buffer.read ();

  if (data == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unable to allocate memory to read ior: %p\n"),
                        -1);
    }

  this->factory_key_ = ACE_OS::strdup (ACE_TEXT_CHAR_TO_TCHAR(data));

  ior_buffer.alloc ()->free (data);

  return 0;
}

// Parses the command line arguments and returns an error status.

int
PP_Test_Client::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("ovdn:f:k:x"));
  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'v':
        this->only_void_ = 1;
        break;
      case 'o':
        this->only_oneway_ = 1;
        break;
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'n':                 // loop count
        this->loop_count_ =
          (u_int) ACE_OS::atoi (get_opts.opt_arg ());
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
        this->factory_key_ =
          ACE_OS::strdup (get_opts.opt_arg ());
        break;
      case 'x':
        ACE_DEBUG ((LM_DEBUG, "We will shutdown the server\n"));
        this->shutdown_ = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-v]"
                           " [-o]"
                           " [-d]"
                           " [-n loopcount]"
                           " [-f factory-obj-ref-key-file]"
                           " [-k obj-ref-key]"
                           " [-x]"
                           "\n",
                           this->argv_ [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

// Oneway test.

void
PP_Test_Client::send_oneway (void)
{

  try
    {
      ACE_FUNCTION_TIMEPROBE (PP_TEST_CLIENT_SEND_ONEWAY_START);
      this->objref_->send_oneway ();
      this->call_count_++;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from send_oneway");

      this->error_count_++;
    }
}

// Twoway test.

void
PP_Test_Client::send_void (void)
{

  try
    {
      ACE_FUNCTION_TIMEPROBE (PP_TEST_CLIENT_SEND_VOID_START);
      this->objref_->send_void ();
      this->call_count_++;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from send_void");

      this->error_count_++;
    }
}

// Send an octet

// Execute client example code.

int
PP_Test_Client::run ()
{
  if (this->only_void_)
    {
      return this->run_void ();
    }

  if (this->only_oneway_)
    {
      return this->run_oneway ();
    }

  CORBA::ULong i;

  // Show the results one type at a time.

  // VOID
  this->call_count_ = 0;
  this->error_count_ = 0;

  for (i = 0; i < this->loop_count_; i++)
    {
      this->send_void ();
    }

  // ONEWAY
  this->call_count_ = 0;
  this->error_count_ = 0;

  for (i = 0; i < this->loop_count_; i++)
    {
      this->send_oneway ();
    }

  // This causes a memPartFree on VxWorks.
  ACE_FUNCTION_TIMEPROBE (PP_TEST_CLIENT_SERVER_SHUTDOWN_START);
  this->shutdown_server (this->shutdown_);

  return this->error_count_ == 0 ? 0 : 1;
}

int
PP_Test_Client::shutdown_server (int do_shutdown)
{

  try
    {
      if (do_shutdown)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "shutdown on Pluggable_Test object\n"));

          this->objref_->shutdown ();


          ACE_DEBUG ((LM_DEBUG,
                      "server, please ACE_OS::exit"));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from shutdown_server");

      return -1;
    }
  return 0;
}

int
PP_Test_Client::run_oneway (void)
{

  try
    {
      CORBA::ULong i;

      // ONEWAY
      this->call_count_ = 0;
      this->error_count_ = 0;

      for (i = 0; i < this->loop_count_; i++)
        {
          this->send_oneway ();
        }

      if (this->shutdown_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "shutdown on Pluggable_Test object\n"));

          ACE_FUNCTION_TIMEPROBE (PP_TEST_CLIENT_SERVER_SHUTDOWN_START);

          this->objref_->shutdown ();

          ACE_DEBUG ((LM_DEBUG,
                      "server, please ACE_OS::exit"));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from objref_->shutdown");

      return -1;
    }
  return this->error_count_ == 0 ? 0 : 1;
}

int
PP_Test_Client::run_void (void)
{

  try
    {
      CORBA::ULong i;

      // ONEWAY
      this->call_count_ = 0;
      this->error_count_ = 0;

      for (i = 0; i < this->loop_count_; i++)
        {
          this->send_void ();
        }

      if (this->shutdown_)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "shutdown on Pluggable_Test object\n"));

          ACE_FUNCTION_TIMEPROBE (PP_TEST_CLIENT_SERVER_SHUTDOWN_START);

          this->objref_->shutdown ();

          ACE_DEBUG ((LM_DEBUG,
                      "server, please ACE_OS::exit"));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("from objref_->shutdown");

      return -1;
    }
  return this->error_count_ == 0 ? 0 : 1;
}

PP_Test_Client::~PP_Test_Client (void)
{
  // Free resources and close the IOR files.
  if (this->factory_ior_file_)
    {
      ACE_OS::fclose (this->factory_ior_file_);
    }

  if (this->f_handle_ != ACE_INVALID_HANDLE)
    {
      ACE_OS::close (this->f_handle_);
    }

  if (this->factory_key_ != 0)
    {
      ACE_OS::free (this->factory_key_);
    }
}

int
PP_Test_Client::init (int argc, ACE_TCHAR **argv)
{
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
        {
          return -1;
        }

      if (this->factory_key_ == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%s: no factory key specified\n",
                             this->argv_[0]),
                            -1);
        }

      CORBA::Object_var factory_object =
        this->orb_->string_to_object (this->factory_key_);

      this->factory_ =
        Pluggable_Test_Factory::_narrow (factory_object.in());

      if (CORBA::is_nil (this->factory_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "invalid factory key <%s>\n",
                             this->factory_key_),
                            -1);
        }

      ACE_DEBUG ((LM_DEBUG,
                  "Factory received OK\n"));

      // Now retrieve the Pluggable_Test obj ref corresponding to the key.
      ACE_FUNCTION_TIMEPROBE (PP_TEST_CLIENT_MAKE_PLUGGABLE_START);

      this->objref_ = this->factory_->make_pluggable_test ();

      if (CORBA::is_nil (this->objref_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "null objref returned by factory\n"),
                            -1);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Pluggable_Test::init");
      return -1;
    }

  return 0;
}
