#include "ClientRequestInterceptor.h"

#include "tao/CORBA_String.h"


ACE_RCSID (PICurrent,
           ClientRequestInterceptor,
           "$Id$")


ClientRequestInterceptor::ClientRequestInterceptor (
  PortableInterceptor::SlotId id,
  PortableInterceptor::Current_ptr pi_current)
  : slot_id_ (id),
    pi_current_ (PortableInterceptor::Current::_duplicate (pi_current))
{
}

char *
ClientRequestInterceptor::name (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("ClientRequestInterceptor");
}

void
ClientRequestInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ClientRequestInterceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (ACE_OS::strcmp (op.in (), "invoke_me") != 0)
    return; // Don't mess with PICurrent if not invoking test method.

  ACE_TRY
    {
      // Retrieve data from the RSC (request scope current).
      CORBA::Long number = 0;

      CORBA::Any_var data =
        ri->get_slot (this->slot_id_
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!(data.in () >>= number))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Unable to extract data from Any.\n"));

          ACE_TRY_THROW (CORBA::INTERNAL ());
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Extracted <%d> from slot %u\n",
                  number,
                  this->slot_id_));

      CORBA::Any new_data;
      CORBA::String_var s = CORBA::string_dup ("Et tu Brute?");

      new_data <<= s.in ();

      // Now reset the contents of our slot in the thread-scope
      // current (TSC).
      this->pi_current_->set_slot (this->slot_id_,
                                   new_data
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Now retrieve the data from the RSC again.  It should not have
      // changed!
      CORBA::Long number2 = -1;

      CORBA::Any_var data2 =
        ri->get_slot (this->slot_id_
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!(data2.in () >>= number2)
          || number != number2)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: RSC was modified after "
                      "TSC was modified.\n"));

          ACE_TRY_THROW (CORBA::INTERNAL ());
        }

    }
  ACE_CATCH (PortableInterceptor::InvalidSlot, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Exception thrown in "
                           "send_request()\n");

      ACE_DEBUG ((LM_DEBUG,
                  "Invalid slot: %u\n",
                  this->slot_id_));

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_ENDTRY;
  ACE_CHECK;

  ACE_DEBUG ((LM_INFO,
              "(%P|%t) Client side RSC/TSC semantics appear "
              "to be correct.\n"));
}

void
ClientRequestInterceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ClientRequestInterceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ClientRequestInterceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
ClientRequestInterceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}
