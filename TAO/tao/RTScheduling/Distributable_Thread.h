//$Id$

#ifndef DISTRIBUTABLE_THREAD_H
#define DISTRIBUTABLE_THREAD_H

#inlcudeRTSchedulerC.h

class TAO_RTScheduler_Export TAO_DistributableThread
:RTScheduling::DistributableThread
{
  virtual void cancel (ACE_ENV_SINGLE_ARG)
    ACE_THROW_SPEC ((CORBA::SystemException));
};
#endif /*DISTRIBUTABLE_THREAD_H*/
