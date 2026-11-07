// file      : RolyPoly/ORB_Initializer.h
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#ifndef ORB_INITIALIZER_H
#define ORB_INITIALIZER_H

#include "tao/LocalObject.h"
#include "tao/PI/PI.h"

class ORB_Initializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual ::CORBA::LocalObject
{
public:
  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;
};

#endif /* ORB_INITIALIZER_H */
