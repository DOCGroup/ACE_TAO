//=============================================================================
/**
 *  @file    Supplier_i.cpp
 *
 *  $Id$
 *
 *  Implementation of the Supplier class.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "Supplier_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Reactor.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_fcntl.h"

// Constructor.

Supplier::Supplier (void)
  : ior_ (0),
    use_naming_service_ (1),
    notifier_ (),
    f_ptr_ (0),
    loop_count_ (10),
    period_value_ (1)
{
  // No-op.
}

Supplier::~Supplier (void)
{
  // Release the memory allocated for ior_.
  ACE_OS::free (this->ior_);

  // Close the stream.
  ACE_OS::fclose (f_ptr_);

  ACE_DEBUG ((LM_DEBUG,
              "Market Status Supplier daemon exiting!\n"));
}

// Reads the Server factory IOR from a file.

int
Supplier::read_ior (ACE_TCHAR *filename)
{
  // Open the file for reading.
  ACE_HANDLE f_handle = ACE_OS::open (filename, 0);

  if (f_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for reading\n",
                        filename),
                        -1);

  ACE_Read_Buffer ior_buffer (f_handle);
  char *data = ior_buffer.read ();

  if (data == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to read ior\n"),
                      -1);

  this->ior_ = ACE_OS::strdup (ACE_TEXT_CHAR_TO_TCHAR(data));
  ior_buffer.alloc ()->free (data);

  ACE_OS::close (f_handle);

  return 0;
}

// Parses the command line arguments and returns an error status.

int
Supplier::parse_args (void)
{
  ACE_Get_Opt get_opts (argc_, argv_, ACE_TEXT("dn:f:i:xk:xs"));

  int c;
  int result;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // Debug flag
        TAO_debug_level++; //****
        break;

      case 'n':  // Period_value: time between two successive stockfeeds.
        this->period_value_ = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':  // Stock market information is got from a file.
        result = this->read_file (get_opts.opt_arg ());
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read stock information from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);
        break;

      case 'k':  // Ior provide on command line
        this->ior_ = ACE_OS::strdup (get_opts.opt_arg ());
        break;

      case 'f': // Read the IOR from the file.
        result = this->read_ior (get_opts.opt_arg ());
        if (result < 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to read ior from %s : %p\n",
                             get_opts.opt_arg ()),
                            -1);
        break;

      case 's': // Don't use the naming service
        this->use_naming_service_ = 0;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n period]"
                           " [-f ior-file]"
                           " [-i input_filename]"
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

// Give the stock status information to the Notifier.

int
Supplier::send_market_status (const char *stock_name,
                              long value)
{

  try
    {

      // Make the RMI.
      this->notifier_->market_status (stock_name,
                                      value);
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception (
        "System Exception : Supplier::send_market_status");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception (
        "User Exception : Supplier::send_market_status");
      return -1;
    }
  return 0;
}

// Execute client example code.

int
Supplier::run (void)
{

  long timer_id = 0;

  ACE_DEBUG ((LM_DEBUG,
              "Market Status Supplier Daemon is running...\n"));

  // This sets the period for the stock-feed.
  ACE_Time_Value period (period_value_);

  // "Your time starts now!" ;) the timer is scheduled to begin work.
  timer_id = reactor_used ()->schedule_timer (supplier_timer_handler_,
                                              "Periodic stockfeed",
                                              period,
                                              period);
  if (timer_id == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "schedule_timer"),
                      -1);

  // The reactor starts executing in a loop.
  return this->reactor_used ()->run_reactor_event_loop ();
}


int
Supplier::via_naming_service (void)
{

  try
    {
      // Initialization of the naming service.
      if (naming_services_client_.init (orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);
      CosNaming::Name notifier_ref_name (1);
      notifier_ref_name.length (1);
      notifier_ref_name[0].id = CORBA::string_dup ("Notifier");

      CORBA::Object_var notifier_obj =
        this->naming_services_client_->resolve (notifier_ref_name);

      // The CORBA::Object_var object is downcast to Notifier_var
      // using the <_narrow> method.
      this->notifier_ =
        Notifier::_narrow (notifier_obj.in ());
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception (
        "System Exception : Supplier::via_naming_service\n");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception (
        "User Exception : Supplier::via_naming_service\n");
      return -1;
    }

  return 0;
}

// Init function.

int
Supplier::init (int argc, ACE_TCHAR **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  try
    {
      // Retrieve the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);

      // Parse command line and verify parameters.
      if (this->parse_args () == -1)
        return -1;

      // Create the Timer_Handler.
      ACE_NEW_RETURN (supplier_timer_handler_,
                      Supplier_Timer_Handler (this,
                                              this->reactor_used (),
                                              this->f_ptr_),
                      -1);

      if (this->use_naming_service_)
        return via_naming_service ();

      if (this->ior_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%s: no ior specified\n",
                           this->argv_[0]),
                          -1);
      CORBA::Object_var notifier_object =
        this->orb_->string_to_object (this->ior_);

      if (CORBA::is_nil (notifier_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "invalid ior <%s>\n",
                           this->ior_),
                          -1);
      // The downcasting from CORBA::Object_var to Notifier_var is
      // done using the <_narrow> method.
      this->notifier_ = Notifier::_narrow (notifier_object.in ());
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception : Supplier::init");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception : Supplier::init");
      return -1;
    }

  return 0;
}

ACE_Reactor*
Supplier::reactor_used (void) const
{
  return ACE_Reactor::instance ();
}

// The stock market information is read from a file.

int
Supplier::read_file (ACE_TCHAR *filename)
{
  f_ptr_ = ACE_OS::fopen (filename, "r");

  ACE_DEBUG ((LM_DEBUG,
              "filename = %s\n",
              filename));

  // the stock values are to be read from a file.
  if (f_ptr_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to open %s for writing: %p\n",
                       filename),
                      -1);
  return 0;
}
