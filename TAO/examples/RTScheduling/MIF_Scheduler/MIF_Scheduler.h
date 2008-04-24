//$Id$

#ifndef MIF_SCHEDULER_H
#define MIF_SCHEDULER_H

#include "../MIF_SchedulingC.h"

#include "tao/orbconf.h"
#include "tao/LocalObject.h"
#include "tao/RTScheduling/RTScheduler.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"

#include "ace/Message_Queue.h"
#include "ace/Null_Condition.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ace/Synch_Traits.h"

class DT : public ACE_Message_Block
{
 public:
  DT (TAO_SYNCH_MUTEX &lock,
      int guid);

  void suspend (void);
  void resume (void);

 private:
  TAO_SYNCH_CONDITION dt_cond_;
  int guid_;
  int eligible_;
};

class Segment_Sched_Param_Policy:
public MIF_Scheduling::SegmentSchedulingParameterPolicy,
       public TAO_Local_RefCounted_Object
{
 public:
  virtual CORBA::Short importance (void);

  virtual void importance (CORBA::Short importance);

  CORBA::Policy_ptr copy (void);

  void destroy (void);

 private:
  CORBA::Short importance_;
};

typedef ACE_Message_Queue<ACE_NULL_SYNCH> DT_Message_Queue;

class MIF_Scheduler:
public MIF_Scheduling::MIF_Scheduler,
public TAO_Local_RefCounted_Object
{
 public:

  MIF_Scheduler (CORBA::ORB_ptr orb);

  ~MIF_Scheduler (void);


  virtual MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr
    create_segment_scheduling_parameter (CORBA::Short segment_priority);

  void wait (void);

  void resume_main (void);

  void incr_thr_count (void);

  virtual void begin_new_scheduling_segment (const RTScheduling::Current::IdType & guid,
               const char * name,
               CORBA::Policy_ptr sched_param,
               CORBA::Policy_ptr implicit_sched_param);

  virtual void begin_nested_scheduling_segment (const RTScheduling::Current::IdType & guid,
            const char * name,
            CORBA::Policy_ptr sched_param,
            CORBA::Policy_ptr implicit_sched_param);

  virtual void update_scheduling_segment (const RTScheduling::Current::IdType & guid,
            const char * name,
            CORBA::Policy_ptr sched_param,
            CORBA::Policy_ptr implicit_sched_param);

  virtual void end_scheduling_segment (const RTScheduling::Current::IdType & guid,
               const char * name);

  virtual void end_nested_scheduling_segment (const RTScheduling::Current::IdType & guid,
                const char * name,
                CORBA::Policy_ptr outer_sched_param);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
        RTScheduling::Current::IdType_out guid,
        CORBA::String_out name,
        CORBA::Policy_out sched_param,
        CORBA::Policy_out implicit_sched_param);

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void cancel (const RTScheduling::Current::IdType & guid);

  virtual CORBA::PolicyList * scheduling_policies (void);

  virtual void scheduling_policies (const CORBA::PolicyList & scheduling_policies);

  virtual CORBA::PolicyList * poa_policies (void);

  virtual char * scheduling_discipline_name (void);

  virtual RTScheduling::ResourceManager_ptr create_resource_manager (const char * name,
                     CORBA::Policy_ptr scheduling_parameter);

  virtual void set_scheduling_parameter (PortableServer::Servant & resource,
           const char * name,
           CORBA::Policy_ptr scheduling_parameter);

 private:
  RTScheduling::Current_var current_;
  RTCORBA::PriorityMappingManager_var mapping_manager_;
  TAO_SYNCH_MUTEX lock_;
  TAO_SYNCH_MUTEX wait_lock_;
  TAO_SYNCH_CONDITION wait_cond_;
  DT_Message_Queue ready_que_;
  DT_Message_Queue wait_que_;
  int wait_;
};

#endif //MIF_SCHEDULER_H
