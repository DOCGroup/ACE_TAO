#ifndef EDF_SCHEDULER_H
#define EDF_SCHEDULER_H

#include "tao/RTScheduling/RTScheduler.h"
#include "EDF_SchedulingC.h"
#include "Kokyu/Kokyu_dsrt.h"
#include "Kokyu_dsrt_schedulers_export.h"
#include "tao/LocalObject.h"
#include "ace/Reactor.h"
#include "tao/ORB_Core.h"

struct EDF_Scheduler_Traits
{
  using Guid_t = RTScheduling::Current::IdType;

  using Time_t = TimeBase::TimeT;

  struct _QoSDescriptor_t
  {
    using Importance_t = long;

    using Time_t = TimeBase::TimeT;

    Importance_t importance_;
    Time_t deadline_;
    Time_t period_;
    int task_id_;
  };

  using QoSDescriptor_t = _QoSDescriptor_t;

  using QoSComparator_t = Kokyu::EDF_Comparator<QoSDescriptor_t>;

  class _Guid_Hash
  {
  public:
    u_long operator () (Guid_t const &id)
      {
        return ACE::hash_pjw (reinterpret_cast<char const *> (id.get_buffer ()),
                              id.length ());
      }
  };
  using Guid_Hash = _Guid_Hash;
};

class Kokyu_DSRT_Schedulers_Export EDF_Sched_Param_Policy:
public EDF_Scheduling::SchedulingParameterPolicy,
       public ::CORBA::LocalObject
{
 public:
    EDF_Scheduling::SchedulingParameter value () override;

    void value (EDF_Scheduling::SchedulingParameter const &value) override;

 private:
    EDF_Scheduling::SchedulingParameter value_;
};

class Kokyu_DSRT_Schedulers_Export EDF_Scheduler:
public EDF_Scheduling::Scheduler,
public ::CORBA::LocalObject
{
 public:
  EDF_Scheduler (CORBA::ORB_ptr orb,
                 Kokyu::DSRT_Dispatcher_Impl_t,
                 int ace_sched_policy,
                 int ace_sched_scope);

  ~EDF_Scheduler ();


  EDF_Scheduling::SchedulingParameterPolicy_ptr
    create_scheduling_parameter (
      EDF_Scheduling::SchedulingParameter const &value) override;

  void shutdown ();

  void begin_new_scheduling_segment (RTScheduling::Current::IdType const &guid,
                                     char const *name,
                                     CORBA::Policy_ptr sched_param,
                                     CORBA::Policy_ptr implicit_sched_param) override;

  void begin_nested_scheduling_segment (RTScheduling::Current::IdType const &guid,
                                        char const *name,
                                        CORBA::Policy_ptr sched_param,
                                        CORBA::Policy_ptr implicit_sched_param) override;

  void update_scheduling_segment (RTScheduling::Current::IdType const &guid,
                                  char const *name,
                                  CORBA::Policy_ptr sched_param,
                                  CORBA::Policy_ptr implicit_sched_param) override;

  void end_scheduling_segment (RTScheduling::Current::IdType const &guid,
                               char const *name) override;

  void end_nested_scheduling_segment (RTScheduling::Current::IdType const &guid,
                                      char const *name,
                                      CORBA::Policy_ptr outer_sched_param) override;

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                        RTScheduling::Current::IdType_out guid,
                        CORBA::String_out name,
                        CORBA::Policy_out sched_param,
                        CORBA::Policy_out implicit_sched_param) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void cancel (RTScheduling::Current::IdType const &guid) override;

  CORBA::PolicyList * scheduling_policies () override;

  void scheduling_policies (CORBA::PolicyList const &scheduling_policies) override;

  CORBA::PolicyList * poa_policies () override;

  char * scheduling_discipline_name () override;

  RTScheduling::ResourceManager_ptr create_resource_manager (
    char const *name,
    CORBA::Policy_ptr scheduling_parameter) override;

  void set_scheduling_parameter (PortableServer::Servant &resource,
                                 char const *name,
                                 CORBA::Policy_ptr scheduling_parameter) override;

  Kokyu::DSRT_Dispatcher_Factory<EDF_Scheduler_Traits>::DSRT_Dispatcher_Auto_Ptr
    kokyu_dispatcher_;
 private:
  CORBA::ORB_var orb_;
  IOP::Codec_var codec_;
  RTScheduling::Current_var current_;
//  Kokyu::DSRT_Dispatcher_Factory<EDF_Scheduler_Traits>::DSRT_Dispatcher_Auto_Ptr
//    kokyu_dispatcher_;
  Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type_;
  int ace_sched_policy_;
  int ace_sched_scope_;
};

//extern int ID_BEGIN;

#endif //EDF_SCHEDULER_H
