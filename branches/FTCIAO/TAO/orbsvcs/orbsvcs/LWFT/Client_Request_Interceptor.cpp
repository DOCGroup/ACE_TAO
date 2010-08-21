// -*- C++ -*-
// $Id$

#include "Client_Request_Interceptor.h"

#include "ace/Log_Msg.h"
#include "ace/High_Res_Timer.h"

#include "ForwardingAgent.h"

Client_Request_Interceptor::Client_Request_Interceptor (
  const char *orb_id,
  ForwardingAgent_i *agent)
  : orb_id_ (CORBA::string_dup (orb_id)),
    orb_ (),
    request_count_ (0),
    agent_ (agent),
    last_successful_(0),
    lag_by_one_(0)
{
}

char *
Client_Request_Interceptor::name (void)
{
  return CORBA::string_dup ("Client_Request_Interceptor");
}

void
Client_Request_Interceptor::destroy (void)
{
}

void
Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr /* ri */)
{
  /*
  ACE_DEBUG ((LM_INFO, 
              ACE_TEXT("(%P|%t) Client_Request_Interceptor::send_request (%s)\n"), 
              ri->operation ()));
  */
}

void
Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr  ri )
{
  /*
  const CORBA::ULong tagID = 9654;
  char *tag = 0;

  IOP::TaggedComponent_var mytag = ri->get_effective_component (tagID);
  tag = reinterpret_cast <char *>(mytag->component_data.get_buffer ());

  if((strcmp(tag,"Alpha") == 0) && (!lag_by_one_))
  {
    ++last_successful_;
    if(this->agent_->getRM()->finish_invocation(
        CORBA::string_dup(tag), 2, 0))
    {
      std::cerr << "Client_Request_Interceptor::receive_reply(): "
                << "finish_invocation successful." << std::endl;
    }
    else
    {
      std::cerr << "Client_Request_Interceptor::receive_reply(): "
                << "finish_invocation failed." << std::endl;
    }
  }
  */
  /*
  ACE_DEBUG ((LM_INFO, 
              ACE_TEXT("(%P|%t) Client_Request_Interceptor::receive_reply (%s)\n"), 
              ri->operation ()));
  */
}

void
Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  if (ACE_OS::strcmp (ri->received_exception_id (),
                      "IDL:omg.org/CORBA/COMM_FAILURE:1.0") != 0
      && ACE_OS::strcmp (ri->received_exception_id (),
                      "IDL:omg.org/CORBA/TRANSIENT:1.0") != 0)
    {
      return;
    }
/*
  ACE_DEBUG ((LM_DEBUG,
              "Client_Request_Interceptor::receive_exception - "
	            "caught %s\n",
	            ri->received_exception_id ()));
*/
  const CORBA::ULong tagID = 9654;
  char *tag = 0;

  try
    {
      CORBA::Object_var forward;
      IOP::TaggedComponent_var mytag = ri->get_effective_component(tagID);
      tag = reinterpret_cast <char *>(mytag->component_data.get_buffer());
      
      if(lag_by_one_)
      {
        ACE_High_Res_Timer timer;
        timer.start();
        forward =
          this->agent_->getRM()->prepare_reinvocation(
              CORBA::string_dup(tag), 
              last_successful_, 5, 0);
        timer.stop();
        ACE_Time_Value  tv;
        timer.elapsed_time(tv);
        std::cerr << "prepare_reinvocation overhead = " << tv.msec() << std::endl;
      }
      else
        forward = this->agent_->next_member (tag);
      
      ACE_THROW (PortableInterceptor::ForwardRequest (forward.in ())); 
    }
  catch (CORBA::BAD_PARAM&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Client_Request_Interceptor::receive_exception - "
		              "Tagged Component not found\n"));
    }
}

void
Client_Request_Interceptor::receive_other (
   PortableInterceptor::ClientRequestInfo_ptr /* ri */)
{
  /*
  ACE_DEBUG ((LM_INFO, 
              ACE_TEXT("(%P|%t) Client_Request_Interceptor::receive_other (%s)\n"), 
              ri->operation ()));
  */
}
