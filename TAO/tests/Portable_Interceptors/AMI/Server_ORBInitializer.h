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

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

class Server_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual ::CORBA::LocalObject
{
public:
  Server_ORBInitializer (void);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
};

#include /**/ "ace/post.h"
#endif /* Client_ORBInitializer__h_ */
