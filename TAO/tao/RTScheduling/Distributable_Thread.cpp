#include "tao/RTScheduling/Distributable_Thread.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_DistributableThread::cancel ()
{
  this->state_ = RTScheduling::DistributableThread::CANCELLED;
}

RTScheduling::DistributableThread::DT_State
TAO_DistributableThread::state ()
{
  return this->state_;
}

RTScheduling::DistributableThread_ptr
TAO_DistributableThread_Factory::create_DT ()
{
  TAO_DistributableThread* DT = nullptr;

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
