// $Id$


#include "IIOP_SSL_Connection_Handler.h"
#include "SSLIOP_Current.h"
#include "tao/Timeprobe.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"


ACE_RCSID(TAO_SSLIOP, IIOP_SSL_Connect, "$Id$")


#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_IIOP_SSL_Connect_Timeprobe_Description[] =
{
  "IIOP_SSL_Connection_Handler::handle_input - start",
  "IIOP_SSL_Connection_Handler::handle_input - end",

  "IIOP_SSL_Connection_Handler::handle_locate - start",
  "IIOP_SSL_Connection_Handler::handle_locate - end",

  "IIOP_SSL_Connection_Handler::receive_request - end",

  "IIOP_SSL_Connection_Handler::send_request - start",
  "IIOP_SSL_Connection_Handler::send_request - end"};

enum
{
  // Timeprobe description table start key
  TAO_IIOP_SSL_CONNECTION_HANDLER_HANDLE_INPUT_START = 320,
  TAO_IIOP_SSL_CONNECTION_HANDLER_HANDLE_INPUT_END,

  TAO_IIOP_SSL_CONNECTION_HANDLER_HANDLE_LOCATE_START,
  TAO_IIOP_SSL_CONNECTION_HANDLER_HANDLE_LOCATE_END,

  TAO_IIOP_SSL_CONNECTION_HANDLER_RECEIVE_REQUEST_END

};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_IIOP_SSL_Connect_Timeprobe_Description,
                                  TAO_IIOP_SSL_CONNECTION_HANDLER_HANDLE_INPUT_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_IIOP_SSL_Connection_Handler::
TAO_IIOP_SSL_Connection_Handler (ACE_Thread_Manager *t)
  : TAO_IIOP_Connection_Handler (t)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (this->orb_core () != 0);
}


TAO_IIOP_SSL_Connection_Handler::
TAO_IIOP_SSL_Connection_Handler (TAO_ORB_Core *orb_core,
                                 CORBA::Boolean flag,
                                 void *arg)
  : TAO_IIOP_Connection_Handler (orb_core, flag, arg)
{
}

TAO_IIOP_SSL_Connection_Handler::
~TAO_IIOP_SSL_Connection_Handler (void)
{
}

int
TAO_IIOP_SSL_Connection_Handler::handle_input_i (
  ACE_HANDLE handle,
  ACE_Time_Value *max_wait_time)
{
  int result;

  // Invalidate the TSS SSL session state to make sure that SSL state
  // from a previous SSL connection is not confused with this non-SSL
  // connection.
  TAO_Null_SSL_State_Guard guard (this->orb_core (), result);

  if (result != 0)
    return -1;

  return
    this->TAO_IIOP_Connection_Handler::handle_input_i (handle,
                                                       max_wait_time);
}

// ****************************************************************

TAO_Null_SSL_State_Guard::TAO_Null_SSL_State_Guard (
  TAO_ORB_Core *orb_core,
  int &result)
{
  // Make sure we have a valid reference to the SSLIOP::Current
  // object.
  if (CORBA::is_nil (this->current_.in ()))
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          CORBA::Object_var object =
            orb_core->orb ()->resolve_initial_references (
              "SSLIOPCurrent",
              ACE_TRY_ENV);
          ACE_TRY_CHECK;

          this->current_ = SSLIOP::Current::_narrow (object.in (),
                                                     ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (CORBA::is_nil (this->current_.in ()))
            ACE_TRY_THROW (CORBA::INV_OBJREF ());
        }
      ACE_CATCHANY
        {
          if (TAO_debug_level > 0)
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                 "TAO_Null_SSL_State_Guard: "
                                 "Could not resolve "
                                 "\"SSLIOPCurrent\" object.");

          result = -1;
          return;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }

  TAO_SSLIOP_Current *current =
    ACE_dynamic_cast (TAO_SSLIOP_Current *,
                      this->current_.in ());

  if (current == 0)   // Sanity check
    {
      result = -1;
      return;
    }

  // Invalidate the TSS SSL session state to make sure that SSL state
  // from a previous SSL connection is not confused with this non-SSL
  // connection.
  current->setup (0);

  result = 0;
}

TAO_Null_SSL_State_Guard::~TAO_Null_SSL_State_Guard (void)
{
  TAO_SSLIOP_Current *current =
    ACE_dynamic_cast (TAO_SSLIOP_Current *,
                      this->current_.in ());

  // Restore the previous TSS SSL state.
  if (current != 0)
    current->teardown ();
}
