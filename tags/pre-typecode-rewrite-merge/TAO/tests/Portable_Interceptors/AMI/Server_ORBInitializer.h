/**
 * @file Server_ORBInitializer.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#ifndef Server_ORBInitializer__h_
#define Server_ORBInitializer__h_
#include /**/ "ace/pre.h"

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

class Server_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual TAO_Local_RefCounted_Object
{
public:
  Server_ORBInitializer (void);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#include /**/ "ace/post.h"
#endif /* Client_ORBInitializer__h_ */
