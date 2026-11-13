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
  RTCORBA::Priority value () override;

  void value (RTCORBA::Priority value) override;

  CORBA::Policy_ptr copy () override;

  CORBA::PolicyType policy_type () override;

  void destroy () override;

 private:
  RTCORBA::Priority value_;
};

class Fixed_Priority_Scheduler:
public FP_Scheduling::FP_Scheduler,
public ::CORBA::LocalObject
{
 public:
  Fixed_Priority_Scheduler (CORBA::ORB_ptr orb);

  ~Fixed_Priority_Scheduler ();

  FP_Scheduling::SegmentSchedulingParameterPolicy_ptr
    create_segment_scheduling_parameter (RTCORBA::Priority segment_priority) override;

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

  RTScheduling::ResourceManager_ptr
    create_resource_manager (const char * name,
                             CORBA::Policy_ptr scheduling_parameter) override;

  void set_scheduling_parameter (PortableServer::Servant & resource,
                                         const char * name,
                                         CORBA::Policy_ptr scheduling_parameter) override;

 private:
  RTScheduling::Current_var current_;
};

#endif //FIXED_PRIORITY_SCHEDULER_H
