// -*- C++ -*-
//
// $Id$
//

#ifndef TAO_SERVER_ORB_INITIALIZER_H
#define TAO_SERVER_ORB_INITIALIZER_H

#include <omg/PortableInterceptor.hh>
#include <orbix/corba.hh>

/// Server ORB initializer.
class Server_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual ::CORBA::LocalObject
{
public:
  /// Constructor
  Server_ORBInitializer (void);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
};

#endif /* TAO_SERVER_ORB_INITIALIZER_H */
