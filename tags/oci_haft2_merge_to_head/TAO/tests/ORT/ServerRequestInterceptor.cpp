// -*- C++ -*-

#include "ServerRequestInterceptor.h"

ACE_RCSID (ORT,
           ServerRequestInterceptor,
           "$Id$")


ServerRequestInterceptor::ServerRequestInterceptor (
  const char * orb_id,
  PortableServer::Current_ptr poa_current)
  : orb_id_ (orb_id),
    poa_current_ (PortableServer::Current::_duplicate (poa_current))
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
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
ServerRequestInterceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // If no response is expected, then we're invoking the oneway
  // shutdown operation.  Don't bother displaying output a second
  // time.
  CORBA::Boolean response_expected =
    ri->response_expected (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (!response_expected)
    return;

  PortableServer::POA_var poa;

  ACE_TRY
    {
      poa = this->poa_current_->get_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  ACE_CATCH (PortableServer::Current::NoContext, ex)
    {
      ACE_PRINT_EXCEPTION (ex,
                           "ServerRequestInterceptor::receive_request");

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_ENDTRY;
  ACE_CHECK;

  PortableServer::POA_var parent_poa =
    poa->the_parent (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Make sure there is more than one POA in the POA hierarchy since
  // the servant should have been registered with a child POA, not the
  // RootPOA.
  ACE_ASSERT (!CORBA::is_nil (parent_poa.in ()));

  PortableInterceptor::AdapterName_var name =
    ri->adapter_name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;


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
                  "%s\n",
                  name[i].in ()));
    }

  ACE_DEBUG ((LM_INFO,
              "\n"
              "==============\n"));

  // Make sure the name of the RootPOA is the first in the AdapterName
  // sequence.
  ACE_ASSERT (ACE_OS::strcmp ("RootPOA", name[(CORBA::ULong) 0]) == 0);

  CORBA::String_var orb_id = ri->orb_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (ACE_OS::strcmp (this->orb_id_.in (), orb_id.in ()) == 0);

  CORBA::String_var server_id = ri->server_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (ACE_OS::strcmp (server_id.in (), "ORT_test_server") == 0);
}

void
ServerRequestInterceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
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
