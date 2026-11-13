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

  void suspend ();
  void resume ();

 private:
  TAO_SYNCH_CONDITION dt_cond_;
  int guid_;
  int eligible_;
};

class Segment_Sched_Param_Policy:
public MIF_Scheduling::SegmentSchedulingParameterPolicy,
       public ::CORBA::LocalObject
{
 public:
  CORBA::Short importance () override;

  void importance (CORBA::Short importance) override;

  CORBA::Policy_ptr copy () override;

  CORBA::PolicyType policy_type () override;

  void destroy () override;

 private:
  CORBA::Short importance_;
};

using DT_Message_Queue = ACE_Message_Queue<ACE_NULL_SYNCH>;

class MIF_Scheduler:
public MIF_Scheduling::MIF_Scheduler,
public ::CORBA::LocalObject
{
 public:
  MIF_Scheduler (CORBA::ORB_ptr orb);

  ~MIF_Scheduler ();


  MIF_Scheduling::SegmentSchedulingParameterPolicy_ptr
    create_segment_scheduling_parameter (CORBA::Short segment_priority) override;

  void wait ();

  void resume_main ();

  void incr_thr_count ();

  void begin_new_scheduling_segment (const RTScheduling::Current::IdType & guid,
               const char * name,
               CORBA::Policy_ptr sched_param,
               CORBA::Policy_ptr implicit_sched_param) override;

  void begin_nested_scheduling_segment (const RTScheduling::Current::IdType & guid,
            const char * name,
            CORBA::Policy_ptr sched_param,
            CORBA::Policy_ptr implicit_sched_param) override;

  void update_scheduling_segment (const RTScheduling::Current::IdType & guid,
            const char * name,
            CORBA::Policy_ptr sched_param,
            CORBA::Policy_ptr implicit_sched_param) override;

  void end_scheduling_segment (const RTScheduling::Current::IdType & guid,
               const char * name) override;

  void end_nested_scheduling_segment (const RTScheduling::Current::IdType & guid,
                const char * name,
                CORBA::Policy_ptr outer_sched_param) override;

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
        RTScheduling::Current::IdType_out guid,
        CORBA::String_out name,
        CORBA::Policy_out sched_param,
        CORBA::Policy_out implicit_sched_param) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void cancel (const RTScheduling::Current::IdType & guid) override;

  CORBA::PolicyList * scheduling_policies () override;

  void scheduling_policies (const CORBA::PolicyList & scheduling_policies) override;

  CORBA::PolicyList * poa_policies () override;

  char * scheduling_discipline_name () override;

  RTScheduling::ResourceManager_ptr create_resource_manager (const char * name,
                     CORBA::Policy_ptr scheduling_parameter) override;

  void set_scheduling_parameter (PortableServer::Servant & resource,
           const char * name,
           CORBA::Policy_ptr scheduling_parameter) override;

 private:
  RTScheduling::Current_var current_;
  RTCORBA::PriorityMappingManager_var mapping_manager_;
  TAO_SYNCH_MUTEX lock_;
  TAO_SYNCH_CONDITION wait_cond_;
  DT_Message_Queue ready_que_;
  DT_Message_Queue free_que_;
  int wait_;
};

#endif //MIF_SCHEDULER_H
