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
  ORB_Initializer () = default;
  ORB_Initializer (ORB_Initializer const &) = delete;
  ORB_Initializer (ORB_Initializer &&) = delete;
  ORB_Initializer &operator= (ORB_Initializer const &) = delete;
  ORB_Initializer &operator= (ORB_Initializer &&) = delete;

  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;
};

#endif /* ORB_INITIALIZER_H */
