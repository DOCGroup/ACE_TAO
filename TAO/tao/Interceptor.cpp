// $Id$

// This file defines some default no-op behavior for interceptors.

#include "tao/corbafwd.h"
#include "tao/InterceptorS.h"

//  char * POA_PortableInterceptor::
//  Interceptor::name (CORBA::Environment &)
//  {
//    return CORBA::string_dup ("TAO default");
//  }

void POA_PortableInterceptor::
ServerRequestInterceptor::preinvoke (CORBA::ULong,
                                     CORBA::Boolean,
                                     CORBA::Object_ptr,
                                     const char *,
                                     IOP::ServiceContextList &,
                                     PortableInterceptor::Cookies &,
                                     CORBA::Environment &)
{
  // No-op.
}

void POA_PortableInterceptor::
ServerRequestInterceptor::postinvoke (CORBA::ULong,
                                      CORBA::Boolean,
                                      CORBA::Object_ptr,
                                      const char *,
                                      IOP::ServiceContextList&,
                                      PortableInterceptor::Cookies&,
                                      CORBA::Environment &)
{
  // No-op.
}

void POA_PortableInterceptor::
ServerRequestInterceptor::exception_occurred (CORBA::ULong,
                                              CORBA::Boolean,
                                              CORBA::Object_ptr,
                                              const char *,
                                              //IOP::ServiceContextList &,
                                              //CORBA::Exception_ptr &,
                                              PortableInterceptor::Cookies &,
                                              CORBA::Environment &)
{
  // No-op.
}

void POA_PortableInterceptor::
ClientRequestInterceptor::preinvoke (CORBA::ULong,
                                     CORBA::Boolean,
                                     CORBA::Object_ptr,
                                     const char *,
                                     IOP::ServiceContextList &,
                                     PortableInterceptor::Cookies &,
                                     CORBA::Environment &)
{
  // No-op.
}

void POA_PortableInterceptor::
ClientRequestInterceptor::postinvoke (CORBA::ULong,
                                      CORBA::Boolean,
                                      CORBA::Object_ptr,
                                      const char *,
                                      IOP::ServiceContextList&,
                                      PortableInterceptor::Cookies&,
                                      CORBA::Environment &)
{
  // No-op.
}

void POA_PortableInterceptor::
ClientRequestInterceptor::exception_occurred (CORBA::ULong,
                                              CORBA::Boolean,
                                              CORBA::Object_ptr,
                                              const char *,
                                              //IOP::ServiceContextList &,
                                              //CORBA::Exception_ptr &,
                                              PortableInterceptor::Cookies &,
                                              CORBA::Environment &)
{
  // No-op.
}
