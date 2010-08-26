// -*- C++ -*-
// $Id$

#include "ace/Log_Msg.h"

#include "Current_TestC.h"
#include "Client_Request_Interceptor.h"
#include "Client_ORBInitializer.h"

namespace Test
{

  Client_Request_Interceptor::Client_Request_Interceptor (const char *orb_id,
                                                          TEST test)
    : orb_id_ (CORBA::string_dup (orb_id))
    , request_count_ (0)
    , requestID_ (1)
    , test_ (test)
  {
  }

  CORBA::Long
  Client_Request_Interceptor::interceptions (void)
  {
    return this->request_count_;
  }

  char *
  Client_Request_Interceptor::name (void)
  {
    return CORBA::string_dup ("Client_Request_Interceptor");
  }

  void
  Client_Request_Interceptor::test_transport_current (const ACE_TCHAR* amethod)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("CRI    (%P|%t) Test accessing Transport Current from %s\n"),
                amethod));

    ++this->request_count_;

    int tmp = 0;
    ACE_TCHAR **argv = 0;
    CORBA::ORB_var orb = CORBA::ORB_init (tmp,
                                          argv,
                                          orb_id_.in ());

    // Call the test function, which will throw an exception on
    // failure
    (*this->test_) (orb.in ());

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("CRI    (%P|%t) Successfully tested Transport Current from %s\n"),
                amethod));
  }

  void
  Client_Request_Interceptor::destroy (void)
  {
  }

  void
  Client_Request_Interceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri)
  {
    // Test TC
    test_transport_current (ACE_TEXT ("send_request"));

    CORBA::Boolean const response_expected =
      ri->response_expected ();

    // Oneway?
    if (response_expected)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("CRI    (%P|%t) Sending a two-way\n")));
    else
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("CRI    (%P|%t) Sending a one-way\n")));

    // Make the context to send the context to the target
    IOP::ServiceContext sc;
    sc.context_id = Test::Transport::CurrentTest::ContextTag;

    // How long can a number really get?
    char temp[32];
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

      ACE_OS::sprintf (temp, "%ld", this->requestID_);
      ++this->requestID_;
    }

    CORBA::ULong string_len = ACE_OS::strlen (temp) + 1;
    CORBA::Octet *buf = CORBA::OctetSeq::allocbuf (string_len);
    ACE_OS::strcpy (reinterpret_cast <char *> (buf), temp);

    sc.context_data.replace (string_len, string_len, buf, 1);

    // Add this context to the service context list.
    ri->add_request_service_context (sc, 0);
  }

  void
  Client_Request_Interceptor::send_poll (PortableInterceptor::ClientRequestInfo_ptr)
  {
    test_transport_current (ACE_TEXT ("send_poll"));
  }

  void
  Client_Request_Interceptor::receive_reply (PortableInterceptor::ClientRequestInfo_ptr)
  {
    test_transport_current (ACE_TEXT ("receive_reply"));
  }

  void
  Client_Request_Interceptor::receive_exception (PortableInterceptor::ClientRequestInfo_ptr)
  {
    test_transport_current (ACE_TEXT ("receive_exception"));
  }

  void
  Client_Request_Interceptor::receive_other (PortableInterceptor::ClientRequestInfo_ptr)
  {
    test_transport_current (ACE_TEXT ("receive_other"));
  }
}
