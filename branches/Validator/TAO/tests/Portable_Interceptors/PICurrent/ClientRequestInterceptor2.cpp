#include "ClientRequestInterceptor2.h"

#include "tao/CORBA_String.h"


ACE_RCSID (PICurrent,
           ClientRequestInterceptor2,
           "$Id$")


ClientRequestInterceptor2::ClientRequestInterceptor2 (
  PortableInterceptor::SlotId id)
  : slot_id_ (id)
{
}

char *
ClientRequestInterceptor2::name (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("ClientRequestInterceptor2");
}

void
ClientRequestInterceptor2::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ClientRequestInterceptor2::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{  
  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (ACE_OS::strcmp (op.in (), "invoke_you") != 0)
    return; // Don't mess with PICurrent if not invoking test method.

  ACE_TRY
    {
      // The goal of this test is to verify that a request scope
      // current was successfully shallow copied from a TSC that
      // itself was shallow copied from a ServerRequestInfo's RSC.
      // Specifically, verify that the RSC->TSC->RSC sequence of
      // copies occurred correctly.

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
              "(%P|%t) RSC->TSC->RSC copying appears to be working.\n"));
}

void
ClientRequestInterceptor2::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ClientRequestInterceptor2::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ClientRequestInterceptor2::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
ClientRequestInterceptor2::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}
