// -*- C++ -*-
// $Id$

#include "ServerRequestInterceptor.h"
#include "tao/CORBA_String.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ServerRequestInterceptor::ServerRequestInterceptor (
  PortableInterceptor::SlotId id,
  PortableInterceptor::Current_ptr pi_current)
  : slot_id_ (id),
    pi_current_ (PortableInterceptor::Current::_duplicate (pi_current))
{
}

char *
ServerRequestInterceptor::name (void)
{
  return CORBA::string_dup ("ServerRequestInterceptor");
}

void
ServerRequestInterceptor::destroy (void)
{
}

void
ServerRequestInterceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "invoke_me") != 0)
    return; // Don't mess with PICurrent if not invoking test method.

  try
    {
      // Insert data into the RSC (request scope current).

      CORBA::Long number = 62;

      CORBA::Any data;
      data <<= number;

      ri->set_slot (this->slot_id_, data);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Inserted number <%d> into RSC.\n",
                  number));
    }
  catch (const PortableInterceptor::InvalidSlot& ex)
    {
      ex._tao_print_exception (
        "Exception thrown in ""receive_request_service_contexts()\n");

      ACE_DEBUG ((LM_DEBUG,
                  "Invalid slot: %u\n",
                  this->slot_id_));

      throw CORBA::INTERNAL ();
    }
}

void
ServerRequestInterceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  try
    {
      CORBA::Any new_data;
      CORBA::Long number = 19;
      new_data <<= number;

      // Set a value in RSC, this should not effect TSC anymore
      ri->set_slot (this->slot_id_,
                    new_data);

      // Now retrieve the data from the TSC again.  It should not have
      // changed to the new value
      CORBA::Any_var data2 =
        this->pi_current_->get_slot (this->slot_id_);

      CORBA::Long number2 = 0;
      if ((data2.in () >>= number2)
          && number == number2)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: TSC was modified because "
                      "RSC was modified.\n"));

          throw CORBA::INTERNAL ();
        }
    }
  catch (const PortableInterceptor::InvalidSlot& ex)
    {
      ex._tao_print_exception ("Exception thrown in ""send_reply()\n");

      ACE_DEBUG ((LM_DEBUG,
                  "Invalid slot: %u\n",
                  this->slot_id_));

      throw CORBA::INTERNAL ();
    }

  ACE_DEBUG ((LM_INFO,
              "(%P|%t) Server side RSC/TSC semantics appear "
              "to be correct.\n"));

}

void
ServerRequestInterceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "invoke_me") != 0)
    return; // Don't mess with PICurrent if not invoking test method.

  try
    {
      CORBA::Any_var data;

      // Retrieve the data stored in the RSC.  This data (a string)
      // should be different from the original data (a CORBA::Long)
      // stored into the RSC by the receive_request_service_contexts()
      // interception point.
      data = ri->get_slot (this->slot_id_);

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

          throw CORBA::INTERNAL ();
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
                                   new_data);

      // Now retrieve the data from the RSC again.  It should not have
      // changed!
      CORBA::Any_var data2 =
        ri->get_slot (this->slot_id_);

      const char *str2 = 0;
      if (!(data2.in () >>= str2)
          || ACE_OS::strcmp (str, str2) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: RSC was modified after "
                      "TSC was modified.\n"));

          throw CORBA::INTERNAL ();
        }
    }
  catch (const PortableInterceptor::InvalidSlot& ex)
    {
      ex._tao_print_exception ("Exception thrown in ""send_reply()\n");

      ACE_DEBUG ((LM_DEBUG,
                  "Invalid slot: %u\n",
                  this->slot_id_));

      throw CORBA::INTERNAL ();
    }

  ACE_DEBUG ((LM_INFO,
              "(%P|%t) Server side RSC/TSC semantics appear "
              "to be correct.\n"));
}

void
ServerRequestInterceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
ServerRequestInterceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}
