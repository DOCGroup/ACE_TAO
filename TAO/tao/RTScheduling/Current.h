// -*- C++ -*-

//=============================================================================
/**
 *  @file   Current.h
 */
//=============================================================================

#ifndef TAO_RTSCHEDULER_CURRENT_H
#define TAO_RTSCHEDULER_CURRENT_H

#include /**/ "ace/pre.h"

#include "tao/RTScheduling/rtscheduler_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/LocalObject.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Task.h"
#include "ace/Atomic_Op.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTScheduler_Current_i;
class TAO_TSS_Resources;

/**
 * @class TAO_DTId_Hash
 *
 * @brief Hashing class for Distributable Thread Ids.
 *
 * Define the hash() method for Object Ids.
 */

typedef TAO::unbounded_value_sequence<CORBA::Octet> IdType;

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
    public ::CORBA::LocalObject
{
 public:
  static std::atomic<long> guid_counter;

  TAO_RTScheduler_Current () = default;
  virtual ~TAO_RTScheduler_Current ();

  void init (TAO_ORB_Core* orb);

  RTCORBA::Priority the_priority () override;

  void the_priority (RTCORBA::Priority the_priority) override;

  void rt_current (RTCORBA::Current_ptr rt_current);

  void begin_scheduling_segment
    (const char * name,
     CORBA::Policy_ptr sched_param,
     CORBA::Policy_ptr implicit_sched_param) override;

  void update_scheduling_segment
    (const char * name,
     CORBA::Policy_ptr sched_param,
     CORBA::Policy_ptr implicit_sched_param) override;

  void end_scheduling_segment (const char * name) override;

  RTScheduling::DistributableThread_ptr
    lookup(const RTScheduling::Current::IdType & id) override;

  // returns a null reference if
  // the distributable thread is
  // not known to the local scheduler
  RTScheduling::DistributableThread_ptr
    spawn (RTScheduling::ThreadAction_ptr start,
           CORBA::VoidData data,
           const char* name,
           CORBA::Policy_ptr sched_param,
           CORBA::Policy_ptr implicit_sched_param,
           CORBA::ULong stack_size,
           RTCORBA::Priority base_priority) override;

  ::RTScheduling::Current::IdType *id () override;

  ::CORBA::Policy_ptr scheduling_parameter () override;

  ::CORBA::Policy_ptr implicit_scheduling_parameter ()  override;

  ::RTScheduling::Current::NameList *
    current_scheduling_segment_names () override;

  TAO_RTScheduler_Current_i* implementation ();

  TAO_RTScheduler_Current_i* implementation (TAO_RTScheduler_Current_i*);

  TAO_ORB_Core* orb ();

  DT_Hash_Map* dt_hash ();

  /**
   * @name Reference Related Methods
   */
  //@{
  typedef TAO_RTScheduler_Current_ptr _ptr_type;
  typedef TAO_RTScheduler_Current_var _var_type;

  static TAO_RTScheduler_Current_ptr _duplicate (TAO_RTScheduler_Current_ptr obj);

  static TAO_RTScheduler_Current_ptr _narrow (CORBA::Object_ptr obj);

  static TAO_RTScheduler_Current_ptr _nil ()
    {
      return nullptr;
    }

  const char* _interface_repository_id () const override;
  //@}

private:
  RTCORBA::Current_var rt_current_;
  TAO_ORB_Core* orb_ {};
  DT_Hash_Map dt_hash_;
};

/**
 * @class TAO_RTScheduler_Current_var
 */
class TAO_RTScheduler_Export TAO_RTScheduler_Current_var
  : private TAO_Base_var
{
public:
  TAO_RTScheduler_Current_var (); // default constructor
  TAO_RTScheduler_Current_var (TAO_RTScheduler_Current_ptr p) : ptr_ (p) {}
  TAO_RTScheduler_Current_var (const TAO_RTScheduler_Current_var &); // copy constructor
  ~TAO_RTScheduler_Current_var (); // destructor

  TAO_RTScheduler_Current_var &operator= (TAO_RTScheduler_Current_ptr);
  TAO_RTScheduler_Current_var &operator= (const TAO_RTScheduler_Current_var &);
  TAO_RTScheduler_Current_ptr operator-> () const;

  operator const TAO_RTScheduler_Current_ptr &() const;
  operator TAO_RTScheduler_Current_ptr &();
  // in, inout, out, _retn
  TAO_RTScheduler_Current_ptr in () const;
  TAO_RTScheduler_Current_ptr &inout ();
  TAO_RTScheduler_Current_ptr &out ();
  TAO_RTScheduler_Current_ptr _retn ();
  TAO_RTScheduler_Current_ptr ptr () const;

  // Hooks used by template sequence and object manager classes
  // for non-defined forward declared interfaces.
  static TAO_RTScheduler_Current_ptr duplicate (TAO_RTScheduler_Current_ptr);
  static void release (TAO_RTScheduler_Current_ptr);
  static TAO_RTScheduler_Current_ptr nil ();
  static TAO_RTScheduler_Current_ptr narrow (CORBA::Object *);
  static CORBA::Object * upcast (void *);

private:
  TAO_RTScheduler_Current_ptr ptr_;
  TAO_RTScheduler_Current_var (const TAO_Base_var &rhs) = delete;
  TAO_RTScheduler_Current_var &operator= (const TAO_Base_var &rhs) = delete;
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
                             TAO_RTScheduler_Current_i* prev_current);

  virtual ~TAO_RTScheduler_Current_i ();

  virtual RTScheduling::DistributableThread_ptr
    spawn (RTScheduling::ThreadAction_ptr start,
           CORBA::VoidData data,
           const char* name,
           CORBA::Policy_ptr sched_param,
           CORBA::Policy_ptr implicit_sched_param,
           CORBA::ULong stack_size,
           RTCORBA::Priority base_priority);

  virtual void begin_scheduling_segment
    (const char * name,
     CORBA::Policy_ptr sched_param,
     CORBA::Policy_ptr implicit_sched_param);

  virtual void update_scheduling_segment
    (const char * name,
     CORBA::Policy_ptr sched_param,
     CORBA::Policy_ptr implicit_sched_param);

  virtual void end_scheduling_segment (const char * name);

  virtual RTScheduling::Current::IdType *id ();


  void id (RTScheduling::Current::IdType guid);

  virtual CORBA::Policy_ptr scheduling_parameter ();

  virtual CORBA::Policy_ptr implicit_scheduling_parameter ();

  void scheduling_parameter (CORBA::Policy_ptr);

  void implicit_scheduling_parameter (CORBA::Policy_ptr);

  virtual RTScheduling::Current::NameList *
    current_scheduling_segment_names ();

  void cancel_thread ();

  void cleanup_DT ();

  void cleanup_current ();

  void delete_all_currents ();

  const char* name (void);
  void name (const char *);

  TAO_ORB_Core* orb ();

  RTScheduling::Scheduler_ptr scheduler ();

  DT_Hash_Map* dt_hash ();

  RTScheduling::DistributableThread_ptr DT ();
  void DT (RTScheduling::DistributableThread_ptr);

private:
  RTScheduling::Scheduler_var scheduler_;
  TAO_ORB_Core* orb_;
  RTScheduling::Current::IdType guid_;
  CORBA::String_var name_;
  CORBA::Policy_var sched_param_;
  CORBA::Policy_var implicit_sched_param_;
  RTScheduling::DistributableThread_var dt_;
  TAO_RTScheduler_Current_i* previous_current_;
  DT_Hash_Map* dt_hash_;
};

// This class provides an entry point for the// new DT.
class DTTask : public ACE_Task <ACE_SYNCH>
{
public:
  DTTask (TAO_ORB_Core* orb,
          DT_Hash_Map* ,
          TAO_RTScheduler_Current_i*,
          RTScheduling::ThreadAction_ptr start,
          CORBA::VoidData data,
          const char* name,
          CORBA::Policy_ptr sched_param,
          CORBA::Policy_ptr implicit_sched_param);

  virtual ~DTTask ();

  int activate_task (RTCORBA::Priority base_priority,
                     CORBA::ULong stack_size);

  int svc () override;

private:
  TAO_ORB_Core* orb_;
  TAO_RTScheduler_Current_i* current_;
  RTScheduling::ThreadAction_var start_;
  CORBA::VoidData data_;
  RTScheduling::Current::IdType guid_;
  CORBA::String_var name_;
  CORBA::Policy_var sched_param_;
  CORBA::Policy_var implicit_sched_param_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_RTSCHEDULER_CURRENT_H*/
