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
