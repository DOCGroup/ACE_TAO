// $Id$

#include "Dynamic_Adapter_Impl.h"

ACE_RCSID(DynamicInterface, TAO_Dynamic_Adapter_Impl, "$Id$")

#if (TAO_HAS_MINIMUM_CORBA == 0)

#include "Request.h"
#include "Server_Request.h"
#include "tao/Invocation.h"

TAO_Dynamic_Adapter_Impl::TAO_Dynamic_Adapter_Impl (void)
{
}

TAO_Dynamic_Adapter_Impl::~TAO_Dynamic_Adapter_Impl (void)
{
}

void 
TAO_Dynamic_Adapter_Impl::create_request (
    CORBA::Object_ptr obj,
    CORBA::ORB_ptr orb,
    const char *operation,
    CORBA::NVList_ptr arg_list,
    CORBA::NamedValue_ptr result,
    CORBA::ExceptionList_ptr exceptions,
    CORBA::Request_ptr &request,
    CORBA::Flags req_flags,
    CORBA_Environment &ACE_TRY_ENV
  )
{
  ACE_NEW_THROW_EX (request,
                    CORBA::Request (obj,
                                    orb,
                                    operation,
                                    arg_list,
                                    result,
                                    req_flags,
                                    exceptions,
                                    ACE_TRY_ENV),
                    CORBA::NO_MEMORY (
                        CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM
                          ),
                        CORBA::COMPLETED_MAYBE
                      ));
}

CORBA::Request_ptr 
TAO_Dynamic_Adapter_Impl::request (CORBA::Object_ptr obj,
                                   CORBA::ORB_ptr orb,
                                   const char *operation,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Request_ptr req = CORBA::Request::_nil ();
  ACE_NEW_THROW_EX (req,
                    CORBA::Request (obj,
                                    orb,
                                    operation,
                                    ACE_TRY_ENV),
                    CORBA::NO_MEMORY (
                        CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM
                          ),
                        CORBA::COMPLETED_MAYBE
                      ));
  ACE_CHECK_RETURN (CORBA::Request::_nil ());

  return req;
}

CORBA::Boolean
TAO_Dynamic_Adapter_Impl::context_is_nil (CORBA::Context_ptr ctx)
{
  return ctx == 0;
}

void
TAO_Dynamic_Adapter_Impl::context_release (CORBA::Context_ptr ctx)
{
  if (ctx != 0)
    {
      ctx->_decr_refcnt ();
    }
}

CORBA::Boolean
TAO_Dynamic_Adapter_Impl::request_is_nil (CORBA::Request_ptr req)
{
  return req == 0;
}

void
TAO_Dynamic_Adapter_Impl::request_release (CORBA::Request_ptr req)
{
  if (req != 0)
    {
      req->_decr_refcnt ();
    }
}

CORBA::Boolean
TAO_Dynamic_Adapter_Impl::server_request_is_nil (CORBA::ServerRequest_ptr req)
{
  return req == 0;
}

void
TAO_Dynamic_Adapter_Impl::server_request_release (CORBA::ServerRequest_ptr req)
{
  if (req != 0)
    {
      req->_decr_refcnt ();
    }
}

void 
TAO_Dynamic_Adapter_Impl::create_exception_list (
    CORBA::ExceptionList_ptr &list,
    CORBA_Environment &ACE_TRY_ENV
  )
{
  ACE_NEW_THROW_EX (list, 
                    CORBA::ExceptionList,
                    CORBA::NO_MEMORY (
                        CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM
                          ),
                        CORBA::COMPLETED_NO
                      ));
}

CORBA::Exception *
TAO_Dynamic_Adapter_Impl::decode_user_exception (
    CORBA::ExceptionList_ptr exceptions,
    TAO_GIOP_Twoway_Invocation *invocation,
    const char *buf,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  for (CORBA::ULong i = 0;
       exceptions != 0 && i < exceptions->count ();
       i++)
    {
      CORBA::TypeCode_ptr tcp = exceptions->item (i, 
                                                  ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      const char *xid = tcp->id (ACE_TRY_ENV);
      ACE_CHECK_RETURN (0);

      if (ACE_OS::strcmp (buf, xid) != 0)
        {
          continue;
        }

      const ACE_Message_Block *cdr = invocation->inp_stream ().start ();

      CORBA_Any any (tcp, 
                     0,
                     invocation->inp_stream ().byte_order (),
                     cdr);

      CORBA_Exception *exception = 0;

      ACE_NEW_RETURN (exception,
                      CORBA_UnknownUserException (any),
                      0);

      return exception;
    }

  return 0;
}

int
TAO_Dynamic_Adapter_Impl::Initializer (void)
{
  ACE_Service_Config::static_svcs ()->insert (
      &ace_svc_desc_TAO_Dynamic_Adapter_Impl
    );
  
  return 0;
}

ACE_STATIC_SVC_DEFINE (
    TAO_Dynamic_Adapter_Impl,
    ACE_TEXT ("Dynamic_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_Dynamic_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_DynamicInterface, TAO_Dynamic_Adapter_Impl)

#endif /* TAO_HAS_MINIMUM_CORBA */
