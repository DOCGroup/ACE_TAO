// $Id$

#include "ClientInterceptor.h"
#include "tao/OctetSeqC.h"
#include "tao/PI/ClientRequestInfo.h"
#include <iostream>

const CORBA::ULong service_ctx_id = 0xdeed;

ClientInterceptor::
ClientInterceptor (IOP::CodecFactory_var cf)
  : myname_ ("Client_Authentication_Interceptor")
{
  std::cout << "Calling ClientInterceptor constructor." << std::endl;

  // Set up a structure that contains information necessary to
  // create a GIOP 1.2 CDR encapsulation Codec.
  IOP::Encoding encoding;
  encoding.format = IOP::ENCODING_CDR_ENCAPS;
  encoding.major_version = 1;
  encoding.minor_version = 2;

  // Obtain the CDR encapsulation Codec.
  this->codec = cf->create_codec (encoding);

}

ClientInterceptor::~ClientInterceptor (void)
{
}

char *
ClientInterceptor::name ()
{
  std::cout << "Calling ClientInterceptor name() method" << std::endl;
  return CORBA::string_dup (this->myname_);
}

void
ClientInterceptor::destroy ()
{
}

void
ClientInterceptor::send_poll (
                              PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling send_poll()." << std::endl;
}


void
ClientInterceptor::send_request (
                                 PortableInterceptor::ClientRequestInfo_ptr ri)
{
  std::cout << "Calling send_request()." << std::endl;

  IOP::ServiceContext sc;
  sc.context_id = service_ctx_id;

  const CORBA::Long gid = 9007;
  std::cout << "GID: " << gid << std::endl;

  CORBA::Any gid_as_any;
  gid_as_any <<= gid;

  sc.context_data = reinterpret_cast<CORBA::OctetSeq&> (
                                         *codec->encode(gid_as_any));

  // Add this context to the service context list.
  ri->add_request_service_context (sc, 0);

}

void
ClientInterceptor::receive_reply (
                                  PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_reply()." << std::endl;
}

void
ClientInterceptor::receive_other (
                                  PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_other()." << std::endl;
}

void
ClientInterceptor::receive_exception (
                                      PortableInterceptor::ClientRequestInfo_ptr ri)
{
  ACE_UNUSED_ARG(ri);
  std::cout << "Calling receive_exception()." << std::endl;
}
