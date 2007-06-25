// file      : RolyPoly/ORB_Initializer.h
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef ORB_INITIALIZER_H
#define ORB_INITIALIZER_H

#include "tao/LocalObject.h"
#include "tao/PI/PI.h"

class ORB_Initializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual TAO_Local_RefCounted_Object
{
public:

  virtual void
  pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void
  post_init (PortableInterceptor::ORBInitInfo_ptr info);
};

#endif /* ORB_INITIALIZER_H */
