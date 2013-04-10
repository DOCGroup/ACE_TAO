// -*- C++ -*-
// $Id$

#include "ServerRequestInterceptor.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ServerRequestInterceptor::ServerRequestInterceptor (
  const char * orb_id,
  PortableServer::Current_ptr poa_current)
  : orb_id_ (orb_id),
    poa_current_ (PortableServer::Current::_duplicate (poa_current))
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
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
ServerRequestInterceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  // If no response is expected, then we're invoking the oneway
  // shutdown operation.  Don't bother displaying output a second
  // time.
  CORBA::Boolean response_expected =
    ri->response_expected ();

  if (!response_expected)
    return;

  PortableServer::POA_var poa;

  try
    {
      poa = this->poa_current_->get_POA ();
    }
  catch (const PortableServer::Current::NoContext& ex)
    {
      ex._tao_print_exception ("ServerRequestInterceptor::receive_request");

      throw CORBA::INTERNAL ();
    }

  PortableServer::POA_var parent_poa =
    poa->the_parent ();

  // Make sure there is more than one POA in the POA hierarchy since
  // the servant should have been registered with a child POA, not the
  // RootPOA.
  ACE_ASSERT (!CORBA::is_nil (parent_poa.in ()));

  PortableInterceptor::AdapterName_var name =
    ri->adapter_name ();


  ACE_DEBUG ((LM_INFO,
              "POA Hierarchy:\n"
              "==============\n"));

  const CORBA::ULong len = name->length ();

  // Make sure there is more than one AdapterName in the AdapterName
  // sequence since the servant should have been registered with a
  // child POA, not the RootPOA.
  ACE_ASSERT (len > 1);

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      for (CORBA::ULong j = 0; j < i; ++j)
        ACE_DEBUG ((LM_INFO, "\t"));

      ACE_DEBUG ((LM_INFO,
                  "%C\n",
                  static_cast<char const*>(name[i])));
    }

  ACE_DEBUG ((LM_INFO,
              "\n"
              "==============\n"));

  // Make sure the name of the RootPOA is the first in the AdapterName
  // sequence.
  ACE_ASSERT (ACE_OS::strcmp ("RootPOA", name[(CORBA::ULong) 0]) == 0);

  CORBA::String_var orb_id = ri->orb_id ();

  ACE_ASSERT (ACE_OS::strcmp (this->orb_id_.in (), orb_id.in ()) == 0);

  CORBA::String_var server_id = ri->server_id ();

  ACE_ASSERT (ACE_OS::strcmp (server_id.in (), "ORT_test_server") == 0);
}

void
ServerRequestInterceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
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
