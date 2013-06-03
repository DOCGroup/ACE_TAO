// $Id$

#include "server_interceptor.h"
#include "tao/OctetSeqC.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

Ping_Death_Request_Interceptor::Ping_Death_Request_Interceptor (int *counter)
  : myname_ ("Ping_Death_Interceptor"),
    counter_ (counter),
    poa_ (PortableServer::POA::_nil())
{
}

Ping_Death_Request_Interceptor::~Ping_Death_Request_Interceptor (void)
{
}

void
Ping_Death_Request_Interceptor::set_poa (PortableServer::POA_ptr poa)
{
  poa_ = PortableServer::POA::_duplicate (poa);
}

char *
Ping_Death_Request_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
Ping_Death_Request_Interceptor::destroy (void)
{
}

void
Ping_Death_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var operation = ri->operation ();

  if (ACE_OS::strcmp (operation.in(), "ping") != 0)
    {
      return;
    }

  if (this->counter_ != 0)
    {
      ++(*counter_);
    }
  if (CORBA::is_nil(this->poa_))
    {
      return;
    }
  try
    {
      this->poa_->destroy (1,1);
    }
  catch (CORBA::Exception &ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P) deactivate raised %s\n",
                  ex._name()));
    }
  throw ::CORBA::TRANSIENT
    ( CORBA::SystemException::_tao_minor_code (TAO_POA_HOLDING, 1),
      CORBA::COMPLETED_NO);

}


void
Ping_Death_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Ping_Death_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}

void
Ping_Death_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}

void
Ping_Death_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}
