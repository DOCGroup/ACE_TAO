// $Id$

#include "FOO_Client_ORBInitializer.h"
#include "FOO_ClientRequestInterceptor.h"

#include "tao/ORB_Constants.h"
#include "tao/CodecFactory/CodecFactory.h"

void
FOO_Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    )
{
}

void
FOO_Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  IOP::CodecFactory_var codec_factory =
    info->codec_factory ();

  // Set up a structure that contains information necessary to
  // create a GIOP 1.2 CDR encapsulation Codec.
  IOP::Encoding encoding;
  encoding.format = IOP::ENCODING_CDR_ENCAPS;
  encoding.major_version = 1;
  encoding.minor_version = 2;

  // Obtain the CDR encapsulation Codec.
  IOP::Codec_var codec =
    codec_factory->create_codec (encoding);

  PortableInterceptor::ClientRequestInterceptor_ptr foo;
  ACE_NEW_THROW_EX (foo,
                    FOO_ClientRequestInterceptor (codec.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ClientRequestInterceptor_var interceptor =
    foo;

  info->add_client_request_interceptor (interceptor.in ());
}
