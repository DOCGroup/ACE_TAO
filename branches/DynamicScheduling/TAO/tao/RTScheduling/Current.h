//$Id$
#ifndef TAO_RTSCHEDULER_CURRENT_H
#define TAO_RTSCHEDULER_CURRENT_H


#include "rtscheduler_export.h"
#include "RTSchedulerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTScheduler_Current_i;
class TAO_TSS_Resources;

class TAO_RTScheduler_Export TAO_RTScheduler_Current: 
public RTScheduling::Current,
  public TAO_Local_RefCounted_Object
{
 public:
  TAO_Scheduler_Current (void);
  
  
  virtual void begin_scheduling_segment
    (const char * name,
     CORBA::Policy_ptr sched_param,
     CORBA::Policy_ptr implicit_sched_param
     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
		     RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE));
    
  virtual void update_scheduling_segment
    (const char * name,
     CORBA::Policy_ptr sched_param,
     CORBA::Policy_ptr implicit_sched_param
     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
		     RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE));

  virtual void end_scheduling_segment
    (const char * name
     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual RTScheduling::DistributableThread_ptr 
    lookup(const RTScheduling::Current::IdType & id
	   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // returns a null reference if
  // the distributable thread is
  // not known to the local scheduler

  virtual RTScheduling::DistributableThread_ptr
    spawn (RTScheduling::ThreadAction_ptr start,
	   CORBA::ULong stack_size,
	   RTCORBA::Priority base_priority
	   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::RTScheduling::Current::IdType * 
    id (ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual ::CORBA::Policy_ptr 
    scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual ::CORBA::Policy_ptr 
    implicit_scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual ::RTScheduling::Current::NameList * 
    current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));  

  TAO_RTScheduler_Current_i* implementation (void);

  TAO_RTScheduler_Current_i* implementation (TAO_RTScheduler_Current_i*);
  
};


class TAO_RTScheduler_Export TAO_RTScheduler_Current_i
{
 public:
  virtual RTScheduling::DistributableThread_ptr
    spawn (RTScheduling::ThreadAction_ptr start,
	   CORBA::ULong stack_size,
	   RTCORBA::Priority base_priority
	   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual void begin_scheduling_segment
    (const char * name,
     CORBA::Policy_ptr sched_param,
     CORBA::Policy_ptr implicit_sched_param
     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
		     RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE));
  
  virtual void update_scheduling_segment
    (const char * name,
     CORBA::Policy_ptr sched_param,
     CORBA::Policy_ptr implicit_sched_param
     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
		     RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE));

  virtual void end_scheduling_segment
    (const char * name
     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual RTScheduling::DistributableThread_ptr 
    lookup(const RTScheduling::Current::IdType & id
	   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // returns a null reference if
  // the distributable thread is
  // not known to the local scheduler

  virtual ::RTScheduling::Current::IdType * 
    id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual ::CORBA::Policy_ptr 
    scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual ::CORBA::Policy_ptr 
    implicit_scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual ::RTScheduling::Current::NameList * 
    current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));  
  
};

#endif /*TAO_RTSCHEDULER_CURRENT_H*/
