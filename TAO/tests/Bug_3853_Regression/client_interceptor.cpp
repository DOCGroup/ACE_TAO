// $Id$

#include "client_interceptor.h"
#include "HelloC.h"
#include "ace/Log_Msg.h"

extern const ACE_TCHAR *server2_ior;
extern CORBA::ORB_var orb;

ACE_RCSID (Service_Context_Manipulation,
           interceptors,
           "$Id$")

const IOP::ServiceId service_id = 0xdeadbeef;

int isRecursive = 0;
extern Demo::HelloWorld_var server1_shuutdownObj;

Echo_Client_Request_Interceptor::
Echo_Client_Request_Interceptor ()
  : myname_ ("Echo_Client_Interceptor")
{
}

Echo_Client_Request_Interceptor::~Echo_Client_Request_Interceptor (void)
{
}

char *
Echo_Client_Request_Interceptor::name (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::destroy (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do Nothing
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  CORBA::String_var operation = ri->operation ();
  if (ACE_OS::strcmp (operation.in (), "shutdown") == 0) {
    return;
  }
  
  //send to another
  isRecursive ++ ;
  if (isRecursive%2 == 1 )
  {
      
    if (isRecursive == 3) {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)Echo_Client_Request_Interceptor::send_request(%d) ")
                            ACE_TEXT ("shutdown server 1 now. \n"), isRecursive));
      server1_shuutdownObj->shutdown();
    }
    
    CORBA::Object_var helloObj = orb->string_to_object(server2_ior);

    Demo::HelloWorld_var hello = Demo::HelloWorld::_narrow(helloObj);

    if (CORBA::is_nil(hello)) {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t)Echo_Client_Request_Interceptor::send_request(%d) ")
                            ACE_TEXT ("hello reference is nil.\n"), isRecursive));
    }
    else {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)Echo_Client_Request_Interceptor::send_request(%d) ")
                            ACE_TEXT ("call server2 HelloWorld::sayHello() \n"), isRecursive));
      const char* pMsg = " server2 say Hello"; 
      hello->sayHello(pMsg) ;
    }
  }
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  
}

void
Echo_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

}

