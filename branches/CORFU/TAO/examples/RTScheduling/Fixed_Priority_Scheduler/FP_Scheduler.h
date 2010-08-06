//$Id$

#ifndef FIXED_PRIORITY_SCHEDULER_H
#define FIXED_PRIORITY_SCHEDULER_H

#include "tao/RTScheduling/RTScheduler.h"
#include "../FP_SchedulingC.h"
#include "tao/LocalObject.h"

class Segment_Sched_Param_Policy:
public FP_Scheduling::SegmentSchedulingParameterPolicy,
       public ::CORBA::LocalObject
{
 public:
  virtual RTCORBA::Priority value (void);

  virtual void value (RTCORBA::Priority value);

  CORBA::Policy_ptr copy (void);

  void destroy (void);

 private:
  RTCORBA::Priority value_;
};

class Fixed_Priority_Scheduler:
public FP_Scheduling::FP_Scheduler,
public ::CORBA::LocalObject
{
 public:

  Fixed_Priority_Scheduler (CORBA::ORB_ptr orb);

  ~Fixed_Priority_Scheduler (void);

  virtual FP_Scheduling::SegmentSchedulingParameterPolicy_ptr
    create_segment_scheduling_parameter (RTCORBA::Priority segment_priority);

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

  virtual RTScheduling::ResourceManager_ptr
    create_resource_manager (const char * name,
                             CORBA::Policy_ptr scheduling_parameter);

  virtual void set_scheduling_parameter (PortableServer::Servant & resource,
                                         const char * name,
                                         CORBA::Policy_ptr scheduling_parameter);

 private:
  RTScheduling::Current_var current_;
  RTCORBA::PriorityMapping* pm_;

};

#endif //FIXED_PRIORITY_SCHEDULER_H
