// -*- C++ -*-

#include "ServerRequestInterceptor.h"

#include "tao/CORBA_String.h"

ACE_RCSID (PICurrent,
           ServerRequestInterceptor,
           "$Id$")

ServerRequestInterceptor::ServerRequestInterceptor (
  PortableInterceptor::SlotId id,
  PortableInterceptor::Current_ptr pi_current)
  : slot_id_ (id),
    pi_current_ (PortableInterceptor::Current::_duplicate (pi_current))
{
}

char *
ServerRequestInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("ServerRequestInterceptor");
}

void
ServerRequestInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ServerRequestInterceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri
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
      // Insert data into the RSC (request scope current).

      CORBA::Long number = 62;

      CORBA::Any data;
      data <<= number;

      ri->set_slot (this->slot_id_, data ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Inserted number <%d> into RSC.\n",
                  number));
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
}

void
ServerRequestInterceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
ServerRequestInterceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (ACE_OS::strcmp (op.in (), "invoke_me") != 0)
    return; // Don't mess with PICurrent if not invoking test method.

  ACE_TRY
    {
      CORBA::Any_var data;

      // Retrieve the data stored in the RSC.  This data (a string)
      // should be different from the original data (a CORBA::Long)
      // stored into the RSC by the receive_request_service_contexts()
      // interception point.
      data = ri->get_slot (this->slot_id_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // The original data in the RSC was of type CORBA::Long.  If the
      // following extraction from the CORBA::Any fails, then the
      // original data in the RSC was not replaced with the data in
      // the TSC after the test method completed.
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

      // Now verify that the RSC is truly independent of the TSC.  In
      // particular, modifying the TSC at this point should not cause
      // the RSC to be modified.
      CORBA::Any new_data;
      CORBA::Long number = 19;

      new_data <<= number;

      // Now reset the contents of our slot in the thread-scope
      // current (TSC).
      this->pi_current_->set_slot (this->slot_id_,
                                   new_data
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Now retrieve the data from the RSC again.  It should not have
      // changed!
      CORBA::Any_var data2 =
        ri->get_slot (this->slot_id_
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      const char *str2 = 0;
      if (!(data2.in () >>= str2)
          || ACE_OS::strcmp (str, str2) != 0)
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
                           "send_reply()\n");

      ACE_DEBUG ((LM_DEBUG,
                  "Invalid slot: %u\n",
                  this->slot_id_));

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_ENDTRY;
  ACE_CHECK;

  ACE_DEBUG ((LM_INFO,
              "(%P|%t) Server side RSC/TSC semantics appear "
              "to be correct.\n"));
}

void
ServerRequestInterceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
ServerRequestInterceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}
