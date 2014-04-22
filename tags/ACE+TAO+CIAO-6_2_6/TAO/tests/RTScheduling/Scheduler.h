//$Id$
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/LocalObject.h"

class TAO_Scheduler:
public RTScheduling::Scheduler,
  public ::CORBA::LocalObject
{
 public:

  TAO_Scheduler (CORBA::ORB_ptr orb);

  ~TAO_Scheduler (void);

  virtual CORBA::PolicyList * scheduling_policies (void);

  virtual void scheduling_policies (const CORBA::PolicyList & scheduling_policies);

  virtual CORBA::PolicyList * poa_policies (void);

  virtual char * scheduling_discipline_name (void);

  virtual RTScheduling::ResourceManager_ptr create_resource_manager (const char * name,
                                                                     CORBA::Policy_ptr scheduling_parameter);

  virtual void set_scheduling_parameter (PortableServer::Servant & resource,
                                         const char * name,
                                         CORBA::Policy_ptr scheduling_parameter);

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
 private:
  RTScheduling::Current_var current_;
};

#endif //SCHEDULER_H
