// $Id$

#include "Dynamic_Adapter_Impl.h"

ACE_RCSID(DynamicInterface, TAO_Dynamic_Adapter_Impl, "$Id$")

#include "Request.h"
#include "Server_Request.h"
#include "tao/Invocation.h"
#include "tao/ORB_Core.h"
#include "tao/Exception.h"

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

void 
TAO_Dynamic_Adapter_Impl::create_unknown_user_typecode (
    CORBA::TypeCode_ptr &tcp,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  // Create the TypeCode for the CORBA_UnknownUserException.

#if defined(ACE_MVS)
  // @@ We need to use a translator to make sure that all TypeCodes
  // are stored in ISO8859 form, the problem is that this hack does
  // not scale as more native sets have to be supported

  ACE_IBM1047_ISO8859 translator;
  TAO_OutputCDR stream (0,
                        ACE_CDR_BYTE_ORDER,
                        TAO_Exceptions::global_allocator_,
                        TAO_Exceptions::global_allocator_,
                        ACE_DEFAULT_CDR_MEMCPY_TRADEOFF,
                        &translator);
#else
  TAO_OutputCDR stream (0,
                        ACE_CDR_BYTE_ORDER,
                        TAO_Exceptions::global_allocator_,
                        TAO_Exceptions::global_allocator_,
                        ACE_DEFAULT_CDR_MEMCPY_TRADEOFF);
#endif /* ACE_MVS */

  const char *interface_id =
    "IDL:omg.org/CORBA/UnknownUserException:1.0";
  const char *name = "UnknownUserException";
  const char *field_name = "exception";

  CORBA::Boolean result = stream.write_octet (TAO_ENCAP_BYTE_ORDER) == 0
    || stream.write_string (interface_id) == 0
    || stream.write_string (name) == 0
    || stream.write_ulong (1L) == 0
    || stream.write_string (field_name) == 0;
  if (result)
    ACE_THROW (CORBA_INITIALIZE ());

  if (!(stream << CORBA::_tc_any))
    ACE_THROW (CORBA_INITIALIZE ());

  ACE_NEW_THROW_EX (tcp,
                    CORBA::TypeCode (CORBA::tk_except,
                                     stream.length (),
                                     stream.buffer (),
                                     1,
                                     sizeof (CORBA_UserException)),
                    CORBA_INITIALIZE ());
}

int
TAO_Dynamic_Adapter_Impl::Initializer (void)
{
  TAO_ORB_Core::dynamic_adapter_name ("Concrete_Dynamic_Adapter");
  ACE_Service_Config::static_svcs ()->insert (
      &ace_svc_desc_TAO_Dynamic_Adapter_Impl
    );
  
  return 0;
}

ACE_STATIC_SVC_DEFINE (
    TAO_Dynamic_Adapter_Impl,
    ACE_TEXT ("Concrete_Dynamic_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_Dynamic_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_DynamicInterface, TAO_Dynamic_Adapter_Impl)

