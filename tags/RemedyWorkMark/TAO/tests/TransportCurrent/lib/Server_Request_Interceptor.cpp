// -*- C++ -*-
// $Id$

#include "ace/Log_Msg.h"

#include "Server_Request_Interceptor.h"
#include "Server_ORBInitializer.h"

namespace Test
{

  Server_Request_Interceptor::Server_Request_Interceptor (const char* orbid, TEST test)
    : request_count_ (0)
    , orb_id_ (::CORBA::string_dup (orbid))
    , test_ (test)
  {
  }

  Server_Request_Interceptor::~Server_Request_Interceptor (void)
  {
  }

  void
  Server_Request_Interceptor::test_transport_current (const ACE_TCHAR* amethod)
  {
    CORBA::String_var name (this->name ());
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%s (%P|%t) Test accessing Transport Current from %s\n"),
                name.in (),
                amethod));

    ++this->request_count_;

     if (TAO_debug_level >=1)
       ACE_DEBUG ((LM_DEBUG,
                   ACE_TEXT ("%s (%P|%t) Getting the ORB\n"),
                   name.in ()));

    int tmpargc = 0;
    ACE_TCHAR **argv = 0;
    CORBA::ORB_var orb = CORBA::ORB_init (tmpargc,
                                          argv,
                                          this->orb_id_.in ());
    if (TAO_debug_level >=1)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s (%P|%t) Resolving the TC\n"),
                  name.in ()));

   CORBA::Object_var tcobject =
     orb->resolve_initial_references ("TAO::Transport::Current");

   // Run the test function
   (*this->test_) (orb.in ());

   ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT ("%s (%P|%t) Success - Transport Current from %s\n"),
               name.in (),
               amethod));
  }

  /// queries the status of the test
  bool
  Server_Request_Interceptor::self_test (void)
  {
    return true;
  }


  char *
  Server_Request_Interceptor::name (void)
  {
    return CORBA::string_dup ("SRI   ");
  }

  void
  Server_Request_Interceptor::destroy (void)
  {
  }

  void
  Server_Request_Interceptor::receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr)
  {
    test_transport_current (ACE_TEXT ("receive_request_service_contexts"));
  }

  void
  Server_Request_Interceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr)
  {
    test_transport_current (ACE_TEXT ("receive_request"));
  }

  void
  Server_Request_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr)
  {
    test_transport_current (ACE_TEXT ("send_reply"));
  }

  void
  Server_Request_Interceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr)
  {
    test_transport_current (ACE_TEXT ("send_exception"));
  }

  void
  Server_Request_Interceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr)
  {
    test_transport_current (ACE_TEXT ("send_other"));
  }
} /* namespace Test */
