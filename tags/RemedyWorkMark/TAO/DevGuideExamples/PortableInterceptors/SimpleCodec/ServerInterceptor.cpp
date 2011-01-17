// $Id$

#include "ServerInterceptor.h"
#include "tao/PI_Server/ServerRequestInfoA.h"
#include "tao/OctetSeqC.h"
#include <iostream>

const IOP::ServiceId service_id = 0xdeed;
const CORBA::Long allowed_gid[4] = { 9006, 9007, 9008 };
const char* restricted_interfaces[1] = {"IDL:Messenger:1.0"};

ServerInterceptor::ServerInterceptor (IOP::CodecFactory_var cf)
  : myname_ ("Server_Authentication_Interceptor")
{
  std::cout << "Calling ServerInterceptor constructor." << std::endl;

  // Set up a structure that contains information necessary to
  // create a GIOP 1.2 CDR encapsulation Codec.
  IOP::Encoding encoding;
  encoding.format = IOP::ENCODING_CDR_ENCAPS;
  encoding.major_version = 1;
  encoding.minor_version = 2;

  // Obtain the CDR encapsulation Codec.
  this->codec = cf->create_codec (encoding);

}

ServerInterceptor::~ServerInterceptor ()
{
}

char *
ServerInterceptor::name ()
{
  std::cout << "Calling ServerInterceptor name() method" << std::endl;
  return CORBA::string_dup (this->myname_);
}

void
ServerInterceptor::destroy ()
{
  std::cout << "Calling destroy()." << std::endl;
}

void
ServerInterceptor::receive_request_service_contexts (
                                                     PortableInterceptor::ServerRequestInfo_ptr)
{
  std::cout << "Calling receive_request_service_contexts()." << std::endl;
}

void
ServerInterceptor::receive_request (
                                    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  bool permission_granted = false;
  std::cout << "Calling receive_request()." << std::endl;

  if (ri->target_is_a(restricted_interfaces[0])){
    IOP::ServiceId id = service_id;
    // Check that the request service context can be retrieved.
    IOP::ServiceContext_var sc =
      ri->get_request_service_context (id);
    // need to construct an octet seq for decoding
    CORBA::OctetSeq ocSeq = CORBA::OctetSeq(
                                            sc->context_data.length(),
                                            sc->context_data.length(),
                                            sc->context_data.get_buffer(),
                                            false);

    CORBA::Any gid_as_any;
    gid_as_any = *codec->decode(ocSeq);

    CORBA::Long gid;
    if (gid_as_any >>= gid) {
      for (int i=0; i<3; ++i) {
        if ( gid == allowed_gid[i] )
          {
            permission_granted = true;
          }
       }
     } else {
       permission_granted = false;
       std::cerr << "Could not extract GID from any." << std::endl;
     }
  }

  if (permission_granted == true) {
    std::cout << "Permission Granted " << std::endl;
  }
  else {
    std::cout << "Permission Denied " << std::endl;;
  }
}

void
ServerInterceptor::send_reply (
                               PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling send_reply()." << std::endl;
}

void
ServerInterceptor::send_exception (
                                   PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling send_exception()." << std::endl;
}

void
ServerInterceptor::send_other (
                               PortableInterceptor::ServerRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling send_other()." << std::endl;
}
