// -*- C++ -*-

#include "ace/Log_Msg.h"
//#include "ace/Global_Macros.h"
#include "tao/ORB_Constants.h"


ACE_RCSID (ForwardRequest,
           Server_Request_Interceptor,
           "$Id$")

#include "Server_Request_Interceptor.h"

namespace Test {

  using namespace TAO;

  // A specialization to handle server-side interceptors
  template <>
  void
  ORBInitializer<PortableInterceptor::ServerRequestInterceptor>::post_init (PortableInterceptor::ORBInitInfo* oii
                                                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    oii->add_server_request_interceptor (this->interceptor_.in ());
  }



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
  Server_Request_Interceptor::test_transport_current (const ACE_TCHAR* amethod
                                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
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
    CORBA::ORB_var orb = CORBA::ORB_init (tmpargc,
                                          0,
                                          this->orb_id_.in ()
                                          ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (TAO_debug_level >=1)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%s (%P|%t) Resolving the TC\n"),
                  name.in ()));

   CORBA::Object_var tcobject =
      orb->resolve_initial_references ("TAO::Transport::Current"
                                       ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    (*this->test_) (orb.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%s (%P|%t) Success - Transport Current from %s\n"),
                name.in (),
                amethod));
  }

  /// queries the status of the test
  bool
  Server_Request_Interceptor::self_test () const
  {
    return true;
  }


  char *
  Server_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return CORBA::string_dup ("SRI    ");
  }

  void
  Server_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  void
  Server_Request_Interceptor::receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr
                                                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    test_transport_current ("receive_request_service_contexts"
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

  }

  void
  Server_Request_Interceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr
                                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    test_transport_current ("receive_request"
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

  }

  void
  Server_Request_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr
                                                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    test_transport_current ("send_reply"
                            ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void
  Server_Request_Interceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr
                                              ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    test_transport_current ("send_exception"
                            ACE_ENV_ARG_PARAMETER);

    ACE_CHECK;
  }

  void
  Server_Request_Interceptor::send_other (PortableInterceptor::ServerRequestInfo_ptr
                                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
  {
    test_transport_current ("send_other"
                            ACE_ENV_ARG_PARAMETER);

    ACE_CHECK;
  }

} /* namespace Test */
