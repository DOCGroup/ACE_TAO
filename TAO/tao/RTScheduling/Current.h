//$Id$
#ifndef TAO_RTSCHEDULER_CURRENT_H
#define TAO_RTSCHEDULER_CURRENT_H


#include "ace/Hash_Map_Manager_T.h"
#include "rtscheduler_export.h"
#include "RTScheduler.h"
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

extern ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> guid_counter;

class TAO_RTScheduler_Export TAO_DTId_Hash
{
public:

  /// Returns hash value.
  u_long operator () (const IdType &id) const;
};


typedef ACE_Hash_Map_Manager_Ex<IdType, 
                                RTScheduling::DistributableThread_var, 
                                TAO_DTId_Hash, 
                                ACE_Equal_To<IdType>, 
                                TAO_SYNCH_MUTEX> 
  DT_Hash_Map;

typedef ACE_Hash_Map_Iterator_Ex<IdType, 
                                 RTScheduling::DistributableThread_var, 
                                 TAO_DTId_Hash, 
                                 ACE_Equal_To<IdType>, 
                                 TAO_SYNCH_MUTEX> 
  DT_Hash_Map_Iterator;

typedef ACE_Hash_Map_Entry<IdType,
                           RTScheduling::DistributableThread_var> 
  DT_Hash_Map_Entry;

class TAO_RTScheduler_Current;
class TAO_RTScheduler_Current_var;

typedef TAO_RTScheduler_Current* TAO_RTScheduler_Current_ptr;

class TAO_RTScheduler_Export TAO_RTScheduler_Current
  : public RTScheduling::Current,
    public TAO_Local_RefCounted_Object
{
 public:
  
  TAO_RTScheduler_Current (void);

  void init (TAO_ORB_Core* orb 
	     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

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
	   ACE_ENV_ARG_DECL_NOT_USED)
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

  TAO_RTScheduler_Current_i* implementation (void);

  TAO_RTScheduler_Current_i* implementation (TAO_RTScheduler_Current_i*);

  TAO_ORB_Core* orb (void);

  DT_Hash_Map* dt_hash (void);

  /**
   * @name Reference Related Methods
   */
  //@{
#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)
  typedef TAO_RTScheduler_Current_ptr _ptr_type;
  typedef TAO_RTScheduler_Current_var _var_type;
#endif /* ! __GNUC__ || g++ >= 2.8 */

  static TAO_RTScheduler_Current_ptr _duplicate (TAO_RTScheduler_Current_ptr obj);

  static TAO_RTScheduler_Current_ptr _narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);


  static TAO_RTScheduler_Current_ptr _unchecked_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);


  static TAO_RTScheduler_Current_ptr _nil (void)
    {
      return (TAO_RTScheduler_Current_ptr)0;
    }

  virtual void *_tao_QueryInterface (ptr_arith_t type);

  virtual const char* _interface_repository_id (void) const;
  //@}

 private:
  RTCORBA::Current_var rt_current_;
  TAO_ORB_Core* orb_;
  DT_Hash_Map dt_hash_;
  
};



/**
 * @class TAO_RTScheduler_Current_var
 */
class TAO_RTScheduler_Export TAO_RTScheduler_Current_var : public TAO_Base_var
{
public:

  TAO_RTScheduler_Current_var (void); // default constructor
  TAO_RTScheduler_Current_var (TAO_RTScheduler_Current_ptr p) : ptr_ (p) {}
  TAO_RTScheduler_Current_var (const TAO_RTScheduler_Current_var &); // copy constructor
  ~TAO_RTScheduler_Current_var (void); // destructor

  TAO_RTScheduler_Current_var &operator= (TAO_RTScheduler_Current_ptr);
  TAO_RTScheduler_Current_var &operator= (const TAO_RTScheduler_Current_var &);
  TAO_RTScheduler_Current_ptr operator-> (void) const;

  operator const TAO_RTScheduler_Current_ptr &() const;
  operator TAO_RTScheduler_Current_ptr &();
  // in, inout, out, _retn
  TAO_RTScheduler_Current_ptr in (void) const;
  TAO_RTScheduler_Current_ptr &inout (void);
  TAO_RTScheduler_Current_ptr &out (void);
  TAO_RTScheduler_Current_ptr _retn (void);
  TAO_RTScheduler_Current_ptr ptr (void) const;

  // Hooks used by template sequence and object manager classes
  // for non-defined forward declared interfaces.
  static TAO_RTScheduler_Current_ptr duplicate (TAO_RTScheduler_Current_ptr);
  static void release (TAO_RTScheduler_Current_ptr);
  static TAO_RTScheduler_Current_ptr nil (void);
  static TAO_RTScheduler_Current_ptr narrow (
      CORBA::Object *
      ACE_ENV_ARG_DECL_NOT_USED
    );
  static CORBA::Object * upcast (void *);

private:

  TAO_RTScheduler_Current_ptr ptr_;
  // Unimplemented - prevents widening assignment.
  TAO_RTScheduler_Current_var (const TAO_Base_var &rhs);
  TAO_RTScheduler_Current_var &operator= (const TAO_Base_var &rhs);

};


class TAO_RTScheduler_Export TAO_RTScheduler_Current_i
{
 public:

  TAO_RTScheduler_Current_i (TAO_ORB_Core* orb,
			     DT_Hash_Map* dt_hash
			     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  TAO_RTScheduler_Current_i (TAO_ORB_Core* orb,
			     DT_Hash_Map* dt_hash,
			     RTScheduling::Current::IdType guid,
			     const char * name,
			     CORBA::Policy_ptr sched_param,
			     CORBA::Policy_ptr implicit_sched_param,
			     RTScheduling::DistributableThread_ptr dt,
			     TAO_RTScheduler_Current_i* prev_current
			     ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual ~TAO_RTScheduler_Current_i (void)
    {
    };

  virtual RTScheduling::DistributableThread_ptr
    spawn (RTScheduling::ThreadAction_ptr start,
	   CORBA::VoidData data,
	   const char* name,
	   CORBA::Policy_ptr sched_param,
	   CORBA::Policy_ptr implicit_sched_param,
	   CORBA::ULong stack_size,
	   RTCORBA::Priority base_priority
	   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
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
    id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

   
  void  id (RTScheduling::Current::IdType guid );
  
  virtual CORBA::Policy_ptr 
    scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual CORBA::Policy_ptr 
    implicit_scheduling_parameter (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));


  void scheduling_parameter (CORBA::Policy_ptr);
  
  void implicit_scheduling_parameter (CORBA::Policy_ptr);
  
  virtual RTScheduling::Current::NameList * 
    current_scheduling_segment_names (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException)); 
  
  void cancel_thread (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  void cleanup_DT (void);

  void cleanup_current (void);		

  void delete_all_currents (void);

  const char* name (void);
  void name (char *);

  TAO_ORB_Core* orb (void);

  RTScheduling::Scheduler_ptr scheduler (void);

  DT_Hash_Map* dt_hash (void);

  RTScheduling::DistributableThread_ptr DT (void);
  void DT (RTScheduling::DistributableThread_ptr);

 private:
  RTScheduling::Scheduler_var scheduler_;
  TAO_ORB_Core* orb_;
  RTScheduling::Current::IdType guid_;
  CORBA::String_var name_;
  CORBA::Policy_ptr sched_param_;
  CORBA::Policy_ptr implicit_sched_param_;
  RTScheduling::DistributableThread_var dt_;
  TAO_RTScheduler_Current_i* previous_current_;
  DT_Hash_Map* dt_hash_;
};

// This class provides an entry point for the// new DT.
class DTTask : public ACE_Task <ACE_SYNCH>
{
public:
  DTTask (//ACE_Thread_Manager manager,
	  TAO_ORB_Core* orb,
	  DT_Hash_Map* dt_hash,
	  TAO_RTScheduler_Current_i*,	  
	  RTScheduling::ThreadAction_ptr start,
	  CORBA::VoidData data,
	  const char* name,
	  CORBA::Policy_ptr sched_param,
	  CORBA::Policy_ptr implicit_sched_param);
	  
  int activate_task (RTCORBA::Priority base_priority,
		     CORBA::ULong stack_size
		     ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  
  virtual int svc (void);

 private:
  //ACE_Thread_Manager* manager_;
  TAO_ORB_Core* orb_;
  DT_Hash_Map* dt_hash_;
  TAO_RTScheduler_Current_i* current_;
  RTScheduling::ThreadAction_var start_;
  CORBA::VoidData data_;
  RTScheduling::Current::IdType guid_;
  CORBA::String_var name_;
  CORBA::Policy_var sched_param_;
  CORBA::Policy_var implicit_sched_param_;
};
#endif /*TAO_RTSCHEDULER_CURRENT_H*/
