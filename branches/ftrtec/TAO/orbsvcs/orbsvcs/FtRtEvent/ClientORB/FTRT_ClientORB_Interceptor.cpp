// $Id$

#include "FTRT_ClientORB_Interceptor.h"
#include "tao/MProfile.h"
#include "tao/Stub.h"

ACE_RCSID (ClientORB,
           FTRT_ClientORB_Interceptor,
           "$Id$")

namespace FTRT {
  const unsigned FT_TRANSACTION_DEPTH = 30;
  const unsigned FT_FORWARD = 32;
};

FTRT_ClientORB_Interceptor::
FTRT_ClientORB_Interceptor (CORBA::Long transaction_depth)
  : myname_ ("FTRT_ClientORB_Interceptor")
  , transaction_depth_(transaction_depth)
{
}

FTRT_ClientORB_Interceptor::~FTRT_ClientORB_Interceptor (void)
{
}

char *
FTRT_ClientORB_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
FTRT_ClientORB_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FTRT_ClientORB_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do Nothing
}

void
FTRT_ClientORB_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_TRACE("FTRT_ClientORB_Interceptor::send_request");
  ACE_TRY
  {
      IOP::ServiceContext_var sc =
        ri->get_request_service_context (FTRT::FT_TRANSACTION_DEPTH
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
  }
  ACE_CATCH (CORBA::BAD_PARAM, ex)
  {
        // Add FT_REQUEST context
        IOP::ServiceContext sc;
        TAO_OutputCDR cdr;

         if ((cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)) ==0)
            ACE_THROW (CORBA::MARSHAL ());

        // Add Transaction Depth Context
        if ((cdr << transaction_depth_) == 0)
            ACE_THROW (CORBA::MARSHAL ());
        sc.context_id = FTRT::FT_TRANSACTION_DEPTH;

        ACE_Message_Block mb;
        ACE_CDR::consolidate(&mb, cdr.begin());
        sc.context_data.replace(mb.length(), &mb);

        ri->add_request_service_context (sc, 0 ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
  }
  ACE_ENDTRY;
}

void
FTRT_ClientORB_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRACE("FTRT_ClientORB_Interceptor::receive_reply");

  IOP::ServiceContext_var service_context;
  ACE_TRY {
    service_context =
      ri->get_reply_service_context(FTRT::FT_FORWARD
                                    ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY {
    return;
  }
  ACE_ENDTRY;
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG, "FT_FORWARD Service Context received\n"));

  const char * buf =
    ACE_reinterpret_cast (const char *,
    service_context->context_data.get_buffer ());

  TAO_InputCDR cdr (buf,
    service_context->context_data.length ());

  /*
  CORBA::Boolean byte_order;

  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    ACE_THROW (CORBA::BAD_PARAM ());
  cdr.reset_byte_order (ACE_static_cast (int,byte_order));
  */

  CORBA::Object_var obj;

  if (cdr >> obj) {
    // update the target
     CORBA::Object_var target = ri->target(ACE_ENV_SINGLE_ARG_PARAMETER);
     TAO_MProfile& base_profiles = target->_stubobj ()->base_profiles ();
     base_profiles.set( obj->_stubobj()->base_profiles() );
  }
}

void
FTRT_ClientORB_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
FTRT_ClientORB_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
    ACE_UNUSED_ARG(ri);
}
