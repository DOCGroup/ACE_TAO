// $Id$


#ifndef CRASH_INJECTOR_INITIALIZER__H_
#define CRASH_INJECTOR_INITIALIZER__H_
#include /**/ "ace/pre.h"

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Crash_Injector_Initializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual TAO_Local_RefCounted_Object
{
public:
  Crash_Injector_Initializer ();

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#include /**/ "ace/post.h"
#endif /* CRASH_INJECTOR_INITIALIZER__H_ */


