#include "client_interceptor.h"
#include "HelloC.h"
#include "ace/Log_Msg.h"

#include <cstring>

extern std::basic_string<ACE_TCHAR> const server2_ior;
extern CORBA::ORB_var orb;

int recursive_call_count {};
extern Demo::HelloWorld_var server1_shutdownObj;

Echo_Client_Request_Interceptor::Echo_Client_Request_Interceptor ()
  : myname_ ("Echo_Client_Interceptor")
{
}

char *
Echo_Client_Request_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_.c_str ());
}

void
Echo_Client_Request_Interceptor::destroy ()
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do Nothing
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var const operation = ri->operation ();
  if (std::strcmp (operation.in (), "shutdown") == 0)
    {
      return;
    }

  ++recursive_call_count;
  if (recursive_call_count % 2 == 1)
    {
      if (recursive_call_count == 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Echo_Client_Request_Interceptor::send_request(%d) ")
                      ACE_TEXT ("shutdown server 1 now.\n"),
                      recursive_call_count));
          server1_shutdownObj->shutdown ();
        }

      CORBA::Object_var const hello_obj =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (server2_ior.c_str ()));
      Demo::HelloWorld_var const hello = Demo::HelloWorld::_narrow (hello_obj.in ());
      if (CORBA::is_nil (hello.in ()))
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Echo_Client_Request_Interceptor::send_request(%d) ")
                    ACE_TEXT ("hello reference is nil.\n"),
                    recursive_call_count));
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Echo_Client_Request_Interceptor::send_request(%d) ")
                      ACE_TEXT ("call server2 HelloWorld::sayHello()\n"),
                      recursive_call_count));
          char const *const message = " server2 say Hello";
          hello->sayHello (message);
        }
    }
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Echo_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
