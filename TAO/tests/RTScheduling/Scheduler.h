#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/LocalObject.h"

class TAO_Scheduler:
public RTScheduling::Scheduler,
  public ::CORBA::LocalObject
{
 public:
  explicit TAO_Scheduler (CORBA::ORB_ptr orb);
  TAO_Scheduler (TAO_Scheduler const &) = delete;
  TAO_Scheduler (TAO_Scheduler &&) = delete;
  TAO_Scheduler & operator= (TAO_Scheduler const &) = delete;
  TAO_Scheduler & operator= (TAO_Scheduler &&) = delete;

  ~TAO_Scheduler () override = default;

  CORBA::PolicyList * scheduling_policies () override;

  void scheduling_policies (CORBA::PolicyList const & scheduling_policies) override;

  CORBA::PolicyList * poa_policies () override;

  char * scheduling_discipline_name () override;

  RTScheduling::ResourceManager_ptr create_resource_manager (
    char const * name,
    CORBA::Policy_ptr scheduling_parameter) override;

  void set_scheduling_parameter (PortableServer::Servant & resource,
                                 char const * name,
                                 CORBA::Policy_ptr scheduling_parameter) override;

  void begin_new_scheduling_segment (RTScheduling::Current::IdType const & guid,
                                     char const * name,
                                     CORBA::Policy_ptr sched_param,
                                     CORBA::Policy_ptr implicit_sched_param) override;

  void begin_nested_scheduling_segment (RTScheduling::Current::IdType const & guid,
                                        char const * name,
                                        CORBA::Policy_ptr sched_param,
                                        CORBA::Policy_ptr implicit_sched_param) override;

  void update_scheduling_segment (RTScheduling::Current::IdType const & guid,
                                  char const * name,
                                  CORBA::Policy_ptr sched_param,
                                  CORBA::Policy_ptr implicit_sched_param) override;

  void end_scheduling_segment (RTScheduling::Current::IdType const & guid,
                               char const * name) override;

  void end_nested_scheduling_segment (RTScheduling::Current::IdType const & guid,
                                      char const * name,
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

  void cancel (RTScheduling::Current::IdType const & guid) override;

 private:
  RTScheduling::Current_var current_;
};

#endif //SCHEDULER_H
