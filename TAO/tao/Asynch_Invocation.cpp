// $Id$

#include "tao/Asynch_Invocation.h"
#include "tao/Timeprobe.h"
#include "tao/Stub.h"
#include "tao/Principal.h"
#include "tao/Object_KeyC.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Asynch_Invocation.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Asynch_Invocation, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Asynch_Invocation_Timeprobe_Description[] =
  {
    "GIOP_Asynch_Invocation::invoke - start",
    "GIOP_Asynch_Invocation::invoke - end",

    "GIOP_Asynch_Invocation::start - enter",
    "GIOP_Asynch_Invocation::start - leave",
    "GIOP_Asynch_Invocation::start - connect",
    "GIOP_Asynch_Invocation::start - start_msg",
    "GIOP_Asynch_Invocation::start - request_hdr"
  };

enum
  {
    TAO_GIOP_ASYNCH_INVOCATION_INVOKE_START = 1000,
    TAO_GIOP_ASYNCH_INVOCATION_INVOKE_END,

    TAO_GIOP_ASYNCH_INVOCATION_START_ENTER,
    TAO_GIOP_ASYNCH_INVOCATION_START_LEAVE,
    TAO_GIOP_ASYNCH_INVOCATION_START_CONNECT,
    TAO_GIOP_ASYNCH_INVOCATION_START_START_MSG,
    TAO_GIOP_ASYNCH_INVOCATION_START_REQUEST_HDR
  };


// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Asynch_Invocation_Timeprobe_Description,
                                  TAO_GIOP_ASYNCH_INVOCATION_INVOKE_START);

#endif /* ACE_ENABLE_TIMEPROBES */

void
TAO_GIOP_Twoway_Asynch_Invocation::start (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->TAO_GIOP_Invocation::start (ACE_TRY_ENV);
  ACE_CHECK;

  // If there was a previous reply, cleanup its state first.
  if (this->message_state_->message_size != 0)
    this->message_state_->reset ();

  this->transport_->start_request (this->orb_core_,
                                   this->profile_,
                                   this->opname_,
                                   this->request_id_,
                                   1,
                                   this->out_stream_,
                                   ACE_TRY_ENV);
}

int
TAO_GIOP_Twoway_Asynch_Invocation::invoke (CORBA::ExceptionList &exceptions,
                                           CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,CORBA::UnknownUserException))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_ASYNCH_INVOCATION_INVOKE_START);
  
  int retval = this->invoke_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  // A TAO_INVOKE_EXCEPTION status, but no exception raised means that
  // we have a user exception.
  // @@ This is a bit brittle, think about a better implementation.
  if (retval == TAO_INVOKE_EXCEPTION)
    {
      // Match the exception interface repository id with the
      // exception in the exception list.
      // This is important to decode the exception.

      CORBA::String_var buf;

      // Pull the exception ID out of the marshaling buffer.
      if (this->inp_stream ().read_string (buf.inout ()) == 0)
        {
          ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                            TAO_INVOKE_EXCEPTION);
        }

      for (CORBA::ULong i = 0;
           i < exceptions.count ();
           i++)
        {
          CORBA::TypeCode_ptr tcp = exceptions.item (i, ACE_TRY_ENV);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          const char *xid = tcp->id (ACE_TRY_ENV);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          if (ACE_OS::strcmp (buf.in (), xid) != 0)
            continue;
          
          const ACE_Message_Block* cdr =
            this->inp_stream ().start ();
          CORBA_Any any (tcp, 0, cdr);
          CORBA_Exception *exception;
          ACE_NEW_THROW_EX (exception,
                            CORBA_UnknownUserException (any),
                            CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES));
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          // @@ Think about a better way to raise the exception here,
          //    maybe we need some more macros?
          ACE_TRY_ENV.exception (exception);
          return TAO_INVOKE_EXCEPTION;
        }

      // If we couldn't find the right exception, report it as
      // CORBA::UNKNOWN.

      // @@ It would seem like if the remote exception is a
      //    UserException we can assume that the request was
      //    completed.
      ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                        TAO_INVOKE_EXCEPTION);
    }

  return retval;
}

int
TAO_GIOP_Twoway_Asynch_Invocation::invoke (TAO_Exception_Data *excepts,
                                           CORBA::ULong except_count,
                                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::Exception))
{
  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_ASYNCH_INVOCATION_INVOKE_START);
  
  int retval = this->invoke_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  // A TAO_INVOKE_EXCEPTION status, but no exception raised means that
  // we have a user exception.
  // @@ This is a bit brittle, think about a better implementation.  
  if (retval == TAO_INVOKE_EXCEPTION)
    {
      // Match the exception interface repository id with the
      // exception in the exception list.
      // This is important to decode the exception.

      CORBA::String_var buf;

      // Pull the exception ID out of the marshaling buffer.
      if (this->inp_stream ().read_string (buf.inout ()) == 0)
        {
          ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                            TAO_INVOKE_EXCEPTION);
        }

      for (CORBA::ULong i = 0;
           i < except_count;
           i++)
        {
          CORBA::TypeCode_ptr tcp = excepts[i].tc;
          const char *xid = tcp->id (ACE_TRY_ENV);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          if (ACE_OS::strcmp (buf.in (), xid) != 0)
            continue;

          // Match.
          CORBA::Exception_ptr exception = excepts[i].alloc ();

          if (exception == 0)
            ACE_THROW_RETURN (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                              TAO_INVOKE_EXCEPTION);
          
          this->inp_stream ().decode (exception->_type (),
                                      exception, 0,
                                      ACE_TRY_ENV);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          if (TAO_debug_level > 5)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO: (%P|%t) Raising exception %s\n",
                        buf.in ()));

          // @@ Think about a better way to raise the exception here,
          //    maybe we need some more macros?
          ACE_TRY_ENV.exception (exception);
          return TAO_INVOKE_EXCEPTION;
        }

      // If we couldn't find the right exception, report it as
      // CORBA::UNKNOWN.

      ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE, CORBA::COMPLETED_YES),
                        TAO_INVOKE_EXCEPTION);
    }

  return retval;
}

int
TAO_GIOP_Twoway_Asynch_Invocation::invoke_i (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Register a reply dispatcher for this Asynch_Invocation. Use the
  // heap allocated reply dispatcher.
  
  int retval = this->transport_->bind_reply_dispatcher (this->request_id_,
                                                        this->rd_);
  if (retval == -1)
    {
      // @@ What is the right way to handle this error?
      this->close_connection ();
      ACE_THROW_RETURN (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO),
                        TAO_INVOKE_EXCEPTION);
    }

  // Just send the request, without trying to wait for the reply.
  retval = TAO_GIOP_Invocation::invoke (1, ACE_TRY_ENV);
  ACE_CHECK_RETURN (retval);

  if (retval != TAO_INVOKE_OK)
    return retval;

  // We do not wait for the reply. Let us return. 

  return TAO_INVOKE_OK;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
