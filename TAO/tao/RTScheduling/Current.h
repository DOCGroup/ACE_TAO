//$Id$
#ifndef TAO_RTSCHEDULER_CURRENT_H
#define TAO_RTSCHEDULER_CURRENT_H


#include "ace/Hash_Map_Manager_T.h"
#include "rtscheduler_export.h"
#include "RTSchedulerC.h"
#include "ace/Task.h"
#include "ace/Atomic_Op.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTScheduler_Current_i;
class TAO_TSS_Resources;

/**
 * @class TAO_DTId_Hash
 *
 * @brief Hashing class for Distributable Thread Ids.
 *
 * Define the hash() method for Object Ids.
 */

typedef TAO_Unbounded_Sequence<CORBA::Octet> IdType;

extern ACE_Atomic_Op<ACE_Thread_Mutex, long> guid_counter;

class TAO_RTScheduler_Export TAO_DTId_Hash
{
public:

  /// Returns hash value.
  u_long operator () (const IdType &id) const;
};


typedef ACE_Hash_Map_Manager_Ex<IdType, CORBA::Object_ptr, TAO_DTId_Hash, ACE_Equal_To<IdType>, ACE_Thread_Mutex> DT_Hash_Map;
typedef ACE_Hash_Map_Iterator_Ex<IdType, CORBA::Object_ptr, TAO_DTId_Hash, ACE_Equal_To<IdType>, ACE_Thread_Mutex> DT_Hash_Map_Iterator;
typedef ACE_Hash_Map_Entry <IdType,CORBA::Object_ptr> DT_Hash_Map_Entry;


class TAO_RTScheduler_Export TAO_RTScheduler_Current: 
public RTScheduling::Current,
  public TAO_Local_RefCounted_Object
{
 public:
  
  TAO_RTScheduler_Current (TAO_ORB_Core*);
  
  
  virtual RTCORBA::Priority the_priority (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void the_priority (RTCORBA::Priority the_priority
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void rt_current (RTCORBA::Current_ptr rt_current);
  
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
	   CORBA::VoidData data,
	   const char* name,
	   CORBA::Policy_ptr sched_param,
	   CORBA::Policy_ptr implicit_sched_param,
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

 private:
  RTCORBA::Current_var rt_current_;
  TAO_ORB_Core* orb_;
  DT_Hash_Map dt_hash_;
  
};


class TAO_RTScheduler_Export TAO_RTScheduler_Current_i
{
 public:

  TAO_RTScheduler_Current_i (TAO_ORB_Core* orb,
			     DT_Hash_Map* dt_hash);

  TAO_RTScheduler_Current_i (TAO_ORB_Core* orb,
			     DT_Hash_Map* dt_hash,
			     RTScheduling::Current::IdType guid,
			     const char * name,
			     CORBA::Policy_ptr sched_param,
			     CORBA::Policy_ptr implicit_sched_param,
			     RTScheduling::DistributableThread_ptr dt,
			     TAO_RTScheduler_Current_i* prev_current
			     );

  virtual RTScheduling::DistributableThread_ptr
    spawn (RTScheduling::ThreadAction_ptr start,
	   CORBA::VoidData data,
	   const char* name,
	   CORBA::Policy_ptr sched_param,
	   CORBA::Policy_ptr implicit_sched_param,
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
  
  virtual RTScheduling::Current::IdType * 
    id (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual CORBA::Policy_ptr 
    scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual CORBA::Policy_ptr 
    implicit_scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual RTScheduling::Current::NameList * 
    current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)); 
  
  void cancel_thread (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  void cleanup_DT (void);

  void cleanup_current (void);		

  void delete_all_currents (void);

  const char* name (void);

  TAO_ORB_Core* orb (void);

  RTScheduling::Scheduler_ptr scheduler (void);

  DT_Hash_Map* dt_hash (void);

 private:
  RTScheduling::Scheduler_var scheduler_;
  TAO_ORB_Core* orb_;
  RTScheduling::Current::IdType guid_;
  const char* name_;
  CORBA::Policy_ptr sched_param_;
  CORBA::Policy_ptr implicit_sched_param_;
  RTScheduling::DistributableThread_var dt_;
  TAO_RTScheduler_Current_i* previous_current_;
  DT_Hash_Map* dt_hash_;
};

// This class provides an entry point for the
// new DT.
class DTTask : public ACE_Task <ACE_SYNCH>
{
public:
  DTTask (//ACE_Thread_Manager manager,
	  TAO_ORB_Core* orb,
	  DT_Hash_Map* dt_hash,
	  RTScheduling::ThreadAction_ptr start,
	  CORBA::VoidData data,
	  RTScheduling::Current::IdType guid,
	  const char* name,
	  CORBA::Policy_ptr sched_param,
	  CORBA::Policy_ptr implicit_sched_param,
	  RTScheduling::DistributableThread_ptr dt);
  
  int activate_task (RTCORBA::Priority base_priority,
		     CORBA::ULong stack_size);

  virtual int svc (void);

 private:
  //ACE_Thread_Manager* manager_;
  TAO_ORB_Core* orb_;
  DT_Hash_Map* dt_hash_;
  RTScheduling::ThreadAction_var start_;
  CORBA::VoidData data_;
  RTScheduling::Current::IdType guid_;
  const char* name_;
  CORBA::Policy_var sched_param_;
  CORBA::Policy_var implicit_sched_param_;
  RTScheduling::DistributableThread_var dt_;
};
#endif /*TAO_RTSCHEDULER_CURRENT_H*/
