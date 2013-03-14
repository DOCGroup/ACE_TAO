// $Id$

#include "ClientRequestInterceptor.h"
#include "tao/CORBA_String.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ClientRequestInterceptor::ClientRequestInterceptor (
  PortableInterceptor::SlotId id,
  PortableInterceptor::Current_ptr pi_current)
  : slot_id_ (id),
    pi_current_ (PortableInterceptor::Current::_duplicate (pi_current))
{
}

char *
ClientRequestInterceptor::name ()
{
  return CORBA::string_dup ("ClientRequestInterceptor");
}

void
ClientRequestInterceptor::destroy (void)
{
}

void
ClientRequestInterceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "invoke_me") != 0)
    return; // Don't mess with PICurrent if not invoking test method.

  try
    {
      // Retrieve data from the RSC (request scope current).
      CORBA::Long number = 0;

      CORBA::Any_var data =
        ri->get_slot (this->slot_id_);

      if (!(data.in () >>= number))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Unable to extract data from Any.\n"));

          throw CORBA::INTERNAL ();
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Extracted <%d> from RSC slot %u\n",
                  number,
                  this->slot_id_));

      CORBA::Any new_data;
      CORBA::String_var s = CORBA::string_dup ("Et tu Brute?");

      new_data <<= s.in ();

      // Now reset the contents of our slot in the thread-scope
      // current (TSC).
      this->pi_current_->set_slot (this->slot_id_,
                                   new_data);

      // Now retrieve the data from the RSC again.  It should not have
      // changed!
      CORBA::Long number2 = -1;

      CORBA::Any_var data2 =
        ri->get_slot (this->slot_id_);

      if (!(data2.in () >>= number2)
          || number != number2)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: RSC was modified after "
                      "TSC was modified.\n"));

          throw CORBA::INTERNAL ();
        }

    }
  catch (const PortableInterceptor::InvalidSlot& ex)
    {
      ex._tao_print_exception ("Exception thrown in ""send_request()\n");

      ACE_DEBUG ((LM_DEBUG,
                  "Invalid slot: %u\n",
                  this->slot_id_));

      throw CORBA::INTERNAL ();
    }

  ACE_DEBUG ((LM_INFO,
              "(%P|%t) Client side RSC/TSC semantics appear "
              "to be correct.\n"));
}

void
ClientRequestInterceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
ClientRequestInterceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
ClientRequestInterceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
ClientRequestInterceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
