// $Id$

#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Interceptor.h"
#include "tao/MProfile.h"
#include "tao/Stub.h"
#include "tao/CDR.h"
#include "orbsvcs/Log_Macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace FTRT {
  const unsigned FT_TRANSACTION_DEPTH = 30;
  const unsigned FT_FORWARD = 32;
}

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
FTRT_ClientORB_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
FTRT_ClientORB_Interceptor::destroy (void)
{
}

void
FTRT_ClientORB_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
FTRT_ClientORB_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_TRACE("FTRT_ClientORB_Interceptor::send_request");
  try
  {
    // Add FT_REQUEST context
    IOP::ServiceContext sc;
    TAO_OutputCDR cdr;

    if ((cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)) ==0)
      return;

    // Add Transaction Depth Context
    if ((cdr << transaction_depth_) == 0)
      return;
    sc.context_id = FTRT::FT_TRANSACTION_DEPTH;

    ACE_Message_Block mb;
    ACE_CDR::consolidate(&mb, cdr.begin());
#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
    sc.context_data.replace(mb.length(), &mb);
#else
    // If the replace method is not available, we will need
    // to do the copy manually.  First, set the octet sequence length.
    CORBA::ULong length = mb.length ();
    sc.context_data.length (length);

    // Now copy over each byte.
    char* base = mb.data_block ()->base ();
    for(CORBA::ULong i = 0; i < length; i++)
      {
        sc.context_data[i] = base[i];
      }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */

    ri->add_request_service_context (sc, 0);
  }
  catch (const CORBA::Exception&)
  {
    // Not much can be done anyway. Just keep quiet
  }
}

void
FTRT_ClientORB_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_TRACE("FTRT_ClientORB_Interceptor::receive_reply");


  IOP::ServiceContext_var service_context;
  try{
    service_context =
      ri->get_reply_service_context(FTRT::FT_FORWARD);
  }
  catch (const CORBA::Exception&){
    return;
  }


  const char * buf =
    reinterpret_cast<const char *> (service_context->context_data.get_buffer ());

  TAO_InputCDR cdr (buf,
    service_context->context_data.length ());


  CORBA::Object_var obj;

  if (cdr >> obj) {
    // update the target
     CORBA::Object_var target = ri->target();
     target->_stubobj ()->base_profiles ( obj->_stubobj()->base_profiles() );
     ORBSVCS_DEBUG((LM_DEBUG, "target object updated\n"));
  }
}

void
FTRT_ClientORB_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
FTRT_ClientORB_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr /* ri */)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
