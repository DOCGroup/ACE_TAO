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
TAO_DistributableThread::cancel (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->state_ = RTScheduling::DistributableThread::CANCELLED;
}

RTScheduling::DistributableThread::DT_State
TAO_DistributableThread::state (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->state_;
}

RTScheduling::DistributableThread_ptr
TAO_DistributableThread_Factory::create_DT (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  TAO_DistributableThread* DT = 0;

  ACE_NEW_THROW_EX (DT,
                    TAO_DistributableThread,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  //RTScheduling::DistributableThread_var dt = RTScheduling::DistributableThread::_narrow (DT
  //                                                                             ACE_ENV_ARG_PARAMETER);
  //ACE_CHECK;

  return DT;
}

TAO_END_VERSIONED_NAMESPACE_DECL
