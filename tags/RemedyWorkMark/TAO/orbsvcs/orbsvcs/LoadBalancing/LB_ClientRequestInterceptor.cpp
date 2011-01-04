// $Id$

#include "orbsvcs/LoadBalancing/LB_ClientRequestInterceptor.h"
#include "orbsvcs/CosLoadBalancingC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

char *
TAO_LB_ClientRequestInterceptor::name ()
{
  return CORBA::string_dup ("TAO_LB_ClientRequestInterceptor");
}

void
TAO_LB_ClientRequestInterceptor::destroy (void)
{
}

void
TAO_LB_ClientRequestInterceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  /*
  CORBA::Object_var t = ri->target ();
  */

  // A ServiceContext with an empty payload (i.e. zero length
  // context_data) will be embedded into the request.
  IOP::ServiceContext service_context;
  service_context.context_id = CosLoadBalancing::LOAD_MANAGED;


  /*
  // A CDR encapsulation IOP::Codec could be used here, but it uses
  // interpreted marshaling, which is slower than the compiled
  // marshaling used below.

  TAO_OutputCDR cdr;
  cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);

  // Marshal the target object, i.e. the object reference that points
  // back to the object group into the ServiceContextList.
  cdr << t.in ();

  // TAO extension, replace the contents of the octet sequence with
  // the CDR stream.
  const CORBA::ULong tl = cdr.total_length ();
  service_context.context_data.length (tl);
  CORBA::Octet * buf = service_context.context_data.get_buffer ();
  for (const ACE_Message_Block * i = cdr.begin ();
       i != 0;
       i = i->cont ())
    {
      const size_t len = i->length ();

      // @todo It would be nice to avoid this copy.
      ACE_OS::memcpy (buf, i->rd_ptr (), len);
      buf += len;
    }
  */

  const CORBA::Boolean replace = 0;
  ri->add_request_service_context (service_context, replace);
}

void
TAO_LB_ClientRequestInterceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_LB_ClientRequestInterceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_LB_ClientRequestInterceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_LB_ClientRequestInterceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
