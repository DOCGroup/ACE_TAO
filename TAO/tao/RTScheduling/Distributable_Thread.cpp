//$Id$

#include "Distributable_Thread.h"

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
  TAO_DistributableThread* DT;

  ACE_NEW_THROW_EX (DT,
                    TAO_DistributableThread,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);
  
  //RTScheduling::DistributableThread_var dt = RTScheduling::DistributableThread::_narrow (DT
  //										 ACE_ENV_ARG_PARAMETER);
  //ACE_CHECK;	
  
  return DT;
}





