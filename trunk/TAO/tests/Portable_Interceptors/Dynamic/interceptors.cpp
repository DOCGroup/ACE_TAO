// $Id$

#include "interceptors.h"


ACE_RCSID(Interceptors, interceptors, "$Id$")

#if (TAO_HAS_INTERCEPTORS == 1)
  //const CORBA::ULong request_ctx_id = 0xdead;
  //const CORBA::ULong reply_ctx_id = 0xbeef;
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
                CORBA::Environment &
                )
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do nothing
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

  if (ACE_OS::strcmp (ri->operation (), "normal") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param;
      (paramlist)[0].argument >>= param;

      ACE_DEBUG ((LM_DEBUG,
                  "the arg is %d\n",
                  param));
    }
}

void 
Echo_Client_Request_Interceptor::receive_other (
               PortableInterceptor::ClientRequestInfo_ptr,
               CORBA::Environment &
               )
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
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
   if (ACE_OS::strcmp (ri->operation (), "normal") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param;
      (paramlist)[0].argument >>= param;

      ACE_DEBUG ((LM_DEBUG,
                  "the arg is %d\n",
                  param));
    }
  if (ACE_OS::strcmp (ri->operation (), "calculate") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param1, param2, result;
      (paramlist)[0].argument >>= param1;
      (paramlist)[1].argument >>= param2;
      CORBA_Any result_any = *(ri->result ());
      result_any >>= result;

      ACE_DEBUG ((LM_DEBUG,
                  "the result of calculate  is %d of %d + %d\n",
                  result,
                  param1,
                  param2));
    }

}

void
Echo_Client_Request_Interceptor::receive_exception (PortableInterceptor::ClientRequestInfo_ptr rinfo,
                                                    CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::received_exception from \"%s\" on object: %s\n",
              rinfo->operation (ACE_TRY_ENV),
              this->orb_->object_to_string (rinfo->target ())));

  // As of now, there is no way to extract an exception from an Any in TAO.

  /*  CORBA::Any excp = *(ri->received_exception ());
  CORBA::Exception *e;
  excp >>= *e;*/

  TAO_ClientRequest_Info *ri
    =  ACE_reinterpret_cast (TAO_ClientRequest_Info *,
                             rinfo->_tao_QueryInterface
                             (
                              ACE_reinterpret_cast
                              (
                               ptr_arith_t,
                               &PortableInterceptor::ClientRequestInfo::_narrow
                               )
                              )
                             );

  CORBA::Exception *e = ri->_received_exception ();

  if (ACE_OS::strcmp (ri->received_exception_id (), e->_id ())== 0)
    ACE_DEBUG ((LM_DEBUG,
                "Exception ID=%s\n", e->_id ()));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%s\n"),
              e->_info ().c_str ()));
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
                                                  CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::receive_request from \"%s\"",
              ri->operation ()));

  if (ACE_OS::strcmp (ri->operation (), "normal") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param;
      (paramlist)[0].argument >>= param;

      ACE_DEBUG ((LM_DEBUG,
                  "the arg is %d\n",
                  param));

     }


}

void
Echo_Server_Request_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                                             CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_reply from \"%s\"",
              ri->operation ()));

  if (ACE_OS::strcmp (ri->operation (), "normal") == 0)
    {
     Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param;
      (paramlist)[0].argument >>= param;
      ACE_DEBUG ((LM_DEBUG,
                  "the arg is %d\n",
                  param));
    }

  if (ACE_OS::strcmp (ri->operation (), "calculate") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param1, param2, result;
      (paramlist)[0].argument >>= param1;
      (paramlist)[1].argument >>= param2;

      CORBA_Any result_any = *(ri->result ());
      result_any >>= result;

      ACE_DEBUG ((LM_DEBUG,
                  "the result of calculate  is %d of %d + %d\n",
                  result,
                  param1,
                  param2));
    }
}

void
Echo_Server_Request_Interceptor::send_exception (PortableInterceptor::ServerRequestInfo_ptr rinfo,
                                                 CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_exception from \"%s\"",
              rinfo->operation ()));
  // As of now, there is no way to extract an exception from an Any in TAO.
  /*
  CORBA::Any excp = *(ri->sending_exception ());
  CORBA::Exception *e;
  excp >>= *e;
  */

  TAO_ServerRequest_Info *ri
    =  ACE_reinterpret_cast (TAO_ServerRequest_Info *,
                             rinfo->_tao_QueryInterface
                             (
                              ACE_reinterpret_cast
                              (
                               ptr_arith_t,
                               &PortableInterceptor::ServerRequestInfo::_narrow
                               )
                              )
                             );

  CORBA::Exception *e = ri->_sending_exception ();
  ACE_DEBUG ((LM_DEBUG,
              "Exception ID=%s\n", e->_id ()));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%s\n"),
              e->_info ().c_str ()));

}
#endif /* (TAO_HAS_INTERCEPTORS == 1) */

void 
Echo_Server_Request_Interceptor::receive_request_service_contexts (
            PortableInterceptor::ServerRequestInfo_ptr,
            CORBA::Environment &
            )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   PortableInterceptor::ForwardRequest
                   ))
{
  // Do nothing
}
  
void
Echo_Server_Request_Interceptor::send_other (
                           PortableInterceptor::ServerRequestInfo_ptr,
                           CORBA::Environment &
                           )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableInterceptor::ForwardRequest
      ))
{
  // Do Nothing
}
