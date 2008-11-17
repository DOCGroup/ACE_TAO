// -*- C++ -*-
// $Id$

#include "Client_Request_Interceptor.h"

#include "ace/Log_Msg.h"

#include "ForwardingAgent.h"

Client_Request_Interceptor::Client_Request_Interceptor (
  const char *orb_id,
  ForwardingAgent_i *agent)
  : orb_id_ (CORBA::string_dup (orb_id)),
    orb_ (),
    request_count_ (0),
    agent_ (agent)
{
}

char *
Client_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Client_Request_Interceptor");
}

void
Client_Request_Interceptor::destroy (void)
{
}

void
Client_Request_Interceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_DEBUG ((LM_DEBUG, "Client_Request_Interceptor::receive_exception - "
	      "catching exception\n"));

  const CORBA::ULong tagID = 9654;
  char *tag = 0;
  try
    {
      IOP::TaggedComponent_var mytag = ri->get_effective_component (tagID);
      tag = reinterpret_cast <char *> ( mytag->component_data.get_buffer ());
      ACE_CString new_string = CORBA::string_dup (tag);
      CORBA::Object_var forward = this->agent_->next_member (tag);
      ACE_THROW (PortableInterceptor::ForwardRequest (forward.in ())); 
    }
  catch (CORBA::BAD_PARAM&)
    {
      ACE_DEBUG ((LM_DEBUG, "Client_Request_Interceptor::receive_exception - "
		  "Tagged Component not found\n"));
    }
}

void
Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
