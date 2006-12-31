// -*- C++ -*-

#include "ace/Log_Msg.h"

ACE_RCSID (ForwardRequest,
           Client_Request_Interceptor,
           "$Id$")

#include "Current_TestC.h"
#include "Client_Request_Interceptor.h"
#include "ORBInitializer_T.h"

namespace Test
{

  // A specialization to handle client-side interceptors
  template <>
  void
  ORBInitializer<PortableInterceptor::ClientRequestInterceptor>::post_init (PortableInterceptor::ORBInitInfo* oii
                                                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    oii->add_client_request_interceptor (this->interceptor_.in ());
  }

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
  Client_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup ("Client_Request_Interceptor");
  }

  void
  Client_Request_Interceptor::test_transport_current (const ACE_TCHAR* amethod
                                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CORBA::UserException))
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("CRI    (%P|%t) Test accessing Transport Current from %s\n"),
                amethod));

    ++this->request_count_;

    int tmp = 0;
    CORBA::ORB_var orb = CORBA::ORB_init (tmp,
                                          0,
                                          orb_id_.in ()
                                          ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;


    (*this->test_) (orb.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("CRI    (%P|%t) Successfully tested Transport Current from %s\n"),
                amethod));
  }

  void
  Client_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  void
  Client_Request_Interceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri
                                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    //Test TC

    test_transport_current ("send_request"
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    CORBA::Boolean response_expected =
      ri->response_expected (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    // Oneway?
    if (response_expected)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("CRI    (%P|%t) Sending a two-way\n")));
    else
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("CRI    (%P|%t) Sending a one-way\n")));

    // Make the context to send the context to the target

    IOP::ServiceContext sc;
    sc.context_id = Test::Transport::CurrentTest::ContextTag;

    char temp[32];

    {
      ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

      ACE_OS::sprintf (temp, "%d", this->requestID_);
      this->requestID_++;
    }

    CORBA::ULong string_len = ACE_OS::strlen (temp) + 1;
    CORBA::Octet *buf = CORBA::OctetSeq::allocbuf (string_len);
    ACE_OS::strcpy (reinterpret_cast <char *> (buf), temp);

    sc.context_data.replace (string_len, string_len, buf, 1);

    // Add this context to the service context list.
    ri->add_request_service_context (sc, 0 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

  }

  void
  Client_Request_Interceptor::send_poll (PortableInterceptor::ClientRequestInfo_ptr
                                         ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    test_transport_current ("send_poll"
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  Client_Request_Interceptor::receive_reply (PortableInterceptor::ClientRequestInfo_ptr
                                             ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    test_transport_current ("receive_reply"
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  Client_Request_Interceptor::receive_exception (PortableInterceptor::ClientRequestInfo_ptr
                                                 ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    test_transport_current ("receive_exception"
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  Client_Request_Interceptor::receive_other (PortableInterceptor::ClientRequestInfo_ptr
                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {

    test_transport_current ("receive_other"
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

  }

}
