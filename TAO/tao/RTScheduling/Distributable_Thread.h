// -*- C++ -*-

#ifndef DISTRIBUTABLE_THREAD_H
#define DISTRIBUTABLE_THREAD_H

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTScheduler_Export TAO_DistributableThread:
  public RTScheduling::DistributableThread,
  public ::CORBA::LocalObject
{
public:
  TAO_DistributableThread () = default;
  ~TAO_DistributableThread () = default;

  void cancel () override;

  RTScheduling::DistributableThread::DT_State state () override;

private:
  RTScheduling::DistributableThread::DT_State state_ {RTScheduling::DistributableThread::ACTIVE};
};

class TAO_DistributableThread_Factory
{
public:
  static RTScheduling::DistributableThread_ptr create_DT ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /*DISTRIBUTABLE_THREAD_H*/
