// $Id$

#include "tao/corbafwd.h"
#include "tao/Interceptor.h"

#if !defined (__ACE_INLINE__)
#include "tao/Interceptor.i"
#endif /* defined INLINE */

// Followings are the defualt no-op implementation of client-side and
// server-side interceptors.  The sole purpose to to let user
// overwrite only the interception operations they are interested in
// without providing others.

// ** Users should always provide a name by dupping a string.
//  char * POA_PortableInterceptor::
//  Interceptor::name (CORBA::Environment &)
//  {
//    return CORBA::string_dup ("TAO default");
//  }

#if defined (TAO_HAS_INTERCEPTORS)
void POA_PortableInterceptor::
ServerRequestInterceptor::preinvoke (CORBA::ULong,
                                     CORBA::Boolean,
                                     CORBA::Object_ptr,
                                     const char *,
                                     IOP::ServiceContextList &,
                                     CORBA::NVList_ptr &,
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
                                      CORBA::NVList_ptr &,
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
                                     CORBA::NVList_ptr &,
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
                                      CORBA::NVList_ptr &,
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
#endif /* TAO_HAS_INTERCEPTORS */
