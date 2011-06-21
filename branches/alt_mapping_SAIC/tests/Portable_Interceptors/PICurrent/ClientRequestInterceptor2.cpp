// $Id$

#include "ClientRequestInterceptor2.h"
#include "tao/CORBA_String.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ClientRequestInterceptor2::ClientRequestInterceptor2 (
  PortableInterceptor::SlotId id)
  : slot_id_ (id)
{
}

char *
ClientRequestInterceptor2::name ()
{
  return CORBA::string_dup ("ClientRequestInterceptor2");
}

void
ClientRequestInterceptor2::destroy (void)
{
}

void
ClientRequestInterceptor2::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "invoke_you") != 0)
    return; // Don't mess with PICurrent if not invoking test method.

  try
    {
      // The goal of this test is to verify that a request scope
      // current was successfully shallow copied from a TSC that
      // itself was shallow copied from a ServerRequestInfo's RSC.
      // Specifically, verify that the RSC->TSC->RSC sequence of
      // copies occurred correctly.

      // Retrieve data from the RSC (request scope current).
      CORBA::Long number = 0;

      CORBA::Any_var data =
        ri->get_slot (this->slot_id_);

      if (!(data.in () >>= number))
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: Unable to extract data from "
                      "CORBA::Any retrieved from RSC.\n"));

          throw CORBA::INTERNAL ();
        }

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Extracted <%d> from RSC slot %u\n",
                  number,
                  this->slot_id_));
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
              "(%P|%t) RSC->TSC->RSC copying appears to be working.\n"));
}

void
ClientRequestInterceptor2::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
ClientRequestInterceptor2::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
ClientRequestInterceptor2::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
ClientRequestInterceptor2::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
