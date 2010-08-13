//$Id$

#include "tao/RTScheduling/Distributable_Thread.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DistributableThread::TAO_DistributableThread (void)
  :state_ (RTScheduling::DistributableThread::ACTIVE)
{
}

TAO_DistributableThread::~TAO_DistributableThread (void)
{
}

void
TAO_DistributableThread::cancel (void)
{
  this->state_ = RTScheduling::DistributableThread::CANCELLED;
}

RTScheduling::DistributableThread::DT_State
TAO_DistributableThread::state (void)
{
  return this->state_;
}

RTScheduling::DistributableThread_ptr
TAO_DistributableThread_Factory::create_DT (void)
{
  TAO_DistributableThread* DT = 0;

  ACE_NEW_THROW_EX (DT,
                    TAO_DistributableThread,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return DT;
}

TAO_END_VERSIONED_NAMESPACE_DECL
