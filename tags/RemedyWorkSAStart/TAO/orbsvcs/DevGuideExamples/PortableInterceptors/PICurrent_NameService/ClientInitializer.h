// $Id$

#ifndef CLIENTINITIALIZER_H
#define CLIENTINITIALIZER_H

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/PI/PI.h"
#include "tao/PI/PICurrentC.h"

class ClientInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual ::CORBA::LocalObject
{
 public:
  ClientInitializer (void);
  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);
  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
  void set_slot_data (void);

 private:
   PortableInterceptor::SlotId slot_;
   PortableInterceptor::Current_var current_;
};

#endif

