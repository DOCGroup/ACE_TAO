// -*- C++ -*-

#include "ServerRequestInterceptor.h"

#include "tao/CORBA_String.h"

ACE_RCSID (PICurrent,
           ServerRequestInterceptor,
           "$Id$")

ServerRequestInterceptor::ServerRequestInterceptor (
  PortableInterceptor::SlotId id)
  : slot_id_ (id)
{
}

char *
ServerRequestInterceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("ServerRequestInterceptor");
}

void
ServerRequestInterceptor::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ServerRequestInterceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::String_var op = ri->operation (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (ACE_OS::strcmp (op.in (), "invoke_me") != 0)
    return; // Don't mess with PICurrent if not invoking test method.

  // Insert data into the RSC (request scope current).

  CORBA::Long number = 62;

  CORBA::Any data;
  data <<= number;

  ACE_TRY
    {
      ri->set_slot (this->slot_id_, data TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableInterceptor::InvalidSlot, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Exception thrown in "
                           "receive_request_service_contexts()\n");

      ACE_DEBUG ((LM_DEBUG,
                  "Invalid slot: %u\n",
                  this->slot_id_));

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_ENDTRY;
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Inserted number <%d> into RSC.\n",
              number));
}

void
ServerRequestInterceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
ServerRequestInterceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::String_var op = ri->operation (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (ACE_OS::strcmp (op.in (), "invoke_me") != 0)
    return; // Don't mess with PICurrent if not invoking test method.

  CORBA::Any_var data;

  ACE_TRY
    {
      // Retrieve the data stored in the RSC.  This data should be
      // different from the original data stored into the RSC by the
      // receive_request_service_contexts() interception point.
      data = ri->get_slot (this->slot_id_ TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableInterceptor::InvalidSlot, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "Exception thrown in "
                           "send_reply()\n");

      ACE_DEBUG ((LM_DEBUG,
                  "Invalid slot: %u\n",
                  this->slot_id_));

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_ENDTRY;
  ACE_CHECK;

  // The original data in the RSC was of type CORBA::Long.  If the
  // following extraction from the CORBA::Any fails, then the original
  // data in the RSC was not replaced with the data in the TSC after
  // the test method completed.
  const char *str = 0;
  if (data.in () >>= str)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Retrieved \"%s\" from the RSC.\n",
                  str));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Unable to extract data (a string) "
                  "from the RSC.\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }
}

void
ServerRequestInterceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
ServerRequestInterceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}
