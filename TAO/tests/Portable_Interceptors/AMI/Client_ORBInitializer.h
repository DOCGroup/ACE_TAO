/**
 * @file Client_ORBInitializer.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#ifndef Client_ORBInitializer__h_
#define Client_ORBInitializer__h_
#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

class Client_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual ::CORBA::LocalObject
{
public:
  Client_ORBInitializer () = default;
  Client_ORBInitializer (Client_ORBInitializer const &) = delete;
  Client_ORBInitializer (Client_ORBInitializer &&) = delete;
  Client_ORBInitializer &operator= (Client_ORBInitializer const &) = delete;
  Client_ORBInitializer &operator= (Client_ORBInitializer &&) = delete;

  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;
};

#include /**/ "ace/post.h"
#endif /* Client_ORBInitializer__h_ */
