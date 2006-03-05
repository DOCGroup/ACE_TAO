// -*- C++ -*-

//$Id$

#ifndef DISTRIBUTABLE_THREAD_H
#define DISTRIBUTABLE_THREAD_H

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTScheduler_Export TAO_DistributableThread:
  public RTScheduling::DistributableThread,
  public TAO_Local_RefCounted_Object
{
 public:
  
  TAO_DistributableThread (void);

  ~TAO_DistributableThread (void);

  virtual void cancel (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

   
  virtual RTScheduling::DistributableThread::DT_State state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));
  
 private:
  RTScheduling::DistributableThread::DT_State state_;
};


class TAO_DistributableThread_Factory
{
public:
  static RTScheduling::DistributableThread_ptr create_DT (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /*DISTRIBUTABLE_THREAD_H*/
