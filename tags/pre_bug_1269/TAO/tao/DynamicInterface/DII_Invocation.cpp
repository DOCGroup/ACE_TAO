// $Id$

#include "DII_Invocation.h"
#include "Unknown_User_Exception.h"
#include "tao/Stub.h"
#include "tao/Principal.h"
#include "tao/Object_KeyC.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Transport.h"

#if !defined (__ACE_INLINE__)
# include "DII_Invocation.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(DynamicInterface, DII_Invocation, "$Id$")


int
TAO_GIOP_DII_Invocation::invoke (CORBA::ExceptionList_ptr exceptions
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, CORBA::UnknownUserException))
{
  int retval = this->invoke_i (0
                               ACE_ENV_ARG_PARAMETER);
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

      TAO_InputCDR tmp_stream (this->inp_stream (),
                               this->inp_stream ().start ()->length (),
                               0);

      // Pull the exception ID out of the marshaling buffer.
      if (tmp_stream.read_string (buf.inout ()) == 0)
        {
          ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                            CORBA::COMPLETED_YES),
                            TAO_INVOKE_EXCEPTION);
        }

      for (CORBA::ULong i = 0;
           exceptions != 0 && i < exceptions->count ();
           i++)
        {
          CORBA::TypeCode_ptr tcp = exceptions->item (i
                                                      ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          const char *xid = tcp->id (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (TAO_INVOKE_EXCEPTION);

          if (ACE_OS::strcmp (buf.in (), xid) != 0)
            {
              continue;
            }

          const ACE_Message_Block* cdr = this->inp_stream ().start ();

          CORBA_Any any (tcp,
                         0,
                         this->inp_stream ().byte_order (),
                         cdr);

          ACE_THROW_RETURN (CORBA_UnknownUserException (any),
                            TAO_INVOKE_EXCEPTION);
        }

      // If we couldn't find the right exception, report it as
      // CORBA::UNKNOWN.

      // But first, save the user exception in case we
      // are being used in a TAO gateway.
      this->host_->raw_user_exception (this->inp_stream ());

      // @@ It would seem that if the remote exception is a
      //    UserException we can assume that the request was
      //    completed.
      ACE_THROW_RETURN (CORBA::UNKNOWN (TAO_DEFAULT_MINOR_CODE,
                                        CORBA::COMPLETED_YES),
                        TAO_INVOKE_EXCEPTION);
    }

  return retval;
}

//***************************************************************************

int
TAO_GIOP_DII_Deferred_Invocation::invoke (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->invoke_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}


int
TAO_GIOP_DII_Deferred_Invocation::invoke_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Register a reply dispatcher for this Asynch_Invocation. Use the
  // heap allocated reply dispatcher.

  int retval =
    this->transport_->tms ()->bind_dispatcher (this->op_details_.request_id (),
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
  retval = TAO_GIOP_Invocation::invoke (0
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (retval);

  if (retval != TAO_INVOKE_OK)
    {
      return retval;
    }

  // Everything executed ok; lets remember the transport for later.
  this->rd_->transport (this->transport_);

  // We do not wait for the reply. Let us return.
  return TAO_INVOKE_OK;
}
