// $Id$

#include "interceptors.h"


ACE_RCSID(Interceptors, interceptors, "$Id$")

#if (TAO_HAS_INTERCEPTORS == 1)
const CORBA::ULong request_ctx_id = 0xdead;
const CORBA::ULong reply_ctx_id = 0xbeef;
const char *request_msg = "The Echo_Request_Interceptor request message";
const char *reply_msg = "The Echo_Request_Interceptor reply message";

Echo_Client_Request_Interceptor::Echo_Client_Request_Interceptor (CORBA::ORB_ptr orb)
  : myname_ ("Echo_Client_Interceptor"),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

Echo_Client_Request_Interceptor::~Echo_Client_Request_Interceptor ()
{
}

void
Echo_Client_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Echo_Client_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Echo_Client_Request_Interceptor::name (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void 
Echo_Client_Request_Interceptor::send_poll (
            PortableInterceptor::ClientRequestInfo_ptr,
            CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do Nothing
}

void
Echo_Client_Request_Interceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                                               CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::send_request from \"%s\" on object: %s\n",
              ri->operation (),
              this->orb_->object_to_string (ri->target ())));

  // Populate target member of the ClientRequestInfo.

  // MAke the context to send the context to the target
  IOP::ServiceContext sc;
  sc.context_id = request_ctx_id;

  CORBA::ULong string_len = ACE_OS::strlen (request_msg) + 1;
  CORBA::Octet *buf = 0;
  ACE_NEW (buf,
           CORBA::Octet [string_len]);
  ACE_OS::strcpy (ACE_reinterpret_cast (char *, buf), request_msg);

  sc.context_data.replace (string_len, string_len, buf, 1);

  // Add this context to the service context list.
  ri->add_request_service_context (sc, 0);
}

void
Echo_Client_Request_Interceptor::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
                                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::receive_reply from \"%s\" on object: %s\n",
              ri->operation (ACE_TRY_ENV),
              this->orb_->object_to_string (ri->target ())));

  // ACE_CHECK;

  IOP::ServiceId id = reply_ctx_id;
  IOP::ServiceContext * sc = ri->get_reply_service_context (id);

  if (sc == 0)
    ACE_THROW (CORBA::NO_MEMORY ());

  const char *buf = ACE_reinterpret_cast (const char *, sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Received reply service context: %s\n",
              buf));
}

void 
Echo_Client_Request_Interceptor::receive_other (
            PortableInterceptor::ClientRequestInfo_ptr,
            CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}

void
Echo_Client_Request_Interceptor::receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                                                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::received_exception from \"%s\" on object: %s\n",
              ri->operation (ACE_TRY_ENV),
              this->orb_->object_to_string (ri->target ())));
  ACE_CHECK;

}


Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (CORBA::ORB_ptr orb)
  : myname_ ("Echo_Server_Interceptor"),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

Echo_Server_Request_Interceptor::~Echo_Server_Request_Interceptor ()
{
}

void
Echo_Server_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Echo_Server_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Echo_Server_Request_Interceptor::name (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                                  CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::receive_request from \"%s\"",
              ri->operation ()));

  IOP::ServiceId id = request_ctx_id;
  IOP::ServiceContext *sc = ri->get_request_service_context (id);

  if (sc == 0)
    ACE_THROW (CORBA::NO_MEMORY ());

  const char *buf = ACE_reinterpret_cast (const char *, sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Received service context: %s\n",
              buf));

  // Make the context to send the context to the client
  IOP::ServiceContext scc;

  scc.context_id = reply_ctx_id;

  CORBA::ULong string_len = ACE_OS::strlen (reply_msg) + 1;
  CORBA::Octet *buff = 0;
  ACE_NEW (buff,
           CORBA::Octet [string_len]);

  ACE_OS::strcpy (ACE_reinterpret_cast (char *, buff), reply_msg);

  scc.context_data.replace (string_len, string_len, buff, 1);

  // Add this context to the service context list.
  ri->add_reply_service_context (scc, 0);

}

void 
Echo_Server_Request_Interceptor::receive_request_service_contexts (
             PortableInterceptor::ServerRequestInfo_ptr,
             CORBA::Environment &)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_reply from \"%s\"",
              ri->operation ()));

  IOP::ServiceId id = reply_ctx_id;
  IOP::ServiceContext *sc = ri->get_reply_service_context (id);

  if (sc == 0)
    ACE_THROW (CORBA::NO_MEMORY ());

  const char *buf = ACE_reinterpret_cast (const char *,
                                          sc->context_data.get_buffer ());
  ACE_DEBUG ((LM_DEBUG,
              "  Replying service context: %s\n",
              buf));
}

void
Echo_Server_Request_Interceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                                                 CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_exception from \"%s\"",
              ri->operation ()));
}

void 
Echo_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr,
             CORBA::Environment & )
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}
#endif /* (TAO_HAS_INTERCEPTORS == 1) */
