//$Id$

#include "Distributable_Thread.h"

void 
TAO_DistributableThread::cancel (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}


RTScheduling::DistributableThread_ptr 
TAO_DistributableThread_Factory::create_DT (void)
{
  TAO_DistributableThread* DT;

  ACE_NEW_THROW_EX (DT,
                    TAO_DistributableThread,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  return DT;
}
