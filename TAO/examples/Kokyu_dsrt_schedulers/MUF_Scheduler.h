#ifndef MUF_SCHEDULER_H
#define MUF_SCHEDULER_H

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/CodecFactory/CodecFactory.h"
#include "MUF_SchedulingC.h"
#include "Kokyu/Kokyu_dsrt.h"
#include "Kokyu_dsrt_schedulers_export.h"
#include "tao/LocalObject.h"

struct MUF_Scheduler_Traits
{
  using Guid_t = RTScheduling::Current::IdType;

  using Time_t = TimeBase::TimeT;
  class _Now
  {
  public:
    Time_t operator () ()
      {
        ACE_Time_Value time = ACE_OS::gettimeofday ();
        return 10*time.usec () + 10000000*time.sec ();
      }
  };

  struct _QoSDescriptor_t
  {
    using Criticality_t = long;

    using Time_t = TimeBase::TimeT;
    using Now = _Now;

    Criticality_t criticality_;
    Time_t deadline_;
    Time_t exec_time_;
  };

  using QoSDescriptor_t = _QoSDescriptor_t;

  using QoSComparator_t = Kokyu::MUF_Comparator<QoSDescriptor_t>;

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

class Kokyu_DSRT_Schedulers_Export MUF_Sched_Param_Policy:
public MUF_Scheduling::SchedulingParameterPolicy,
       public ::CORBA::LocalObject
{
 public:
    MUF_Sched_Param_Policy ();
    MUF_Sched_Param_Policy (const MUF_Sched_Param_Policy &rhs);

    MUF_Scheduling::SchedulingParameter value () override;

    void value (const MUF_Scheduling::SchedulingParameter & value) override;

     CORBA::Policy_ptr copy () override;

     CORBA::PolicyType policy_type () override;

     void destroy () override;

 private:
    MUF_Scheduling::SchedulingParameter value_;
};

class Kokyu_DSRT_Schedulers_Export MUF_Scheduler:
public MUF_Scheduling::Scheduler,
public ::CORBA::LocalObject
{
 public:
  MUF_Scheduler (CORBA::ORB_ptr orb,
                 Kokyu::DSRT_Dispatcher_Impl_t,
                 int ace_sched_policy,
                 int ace_sched_scope);

  ~MUF_Scheduler ();


  MUF_Scheduling::SchedulingParameterPolicy_ptr
    create_scheduling_parameter (
      const MUF_Scheduling::SchedulingParameter & value) override;

  void shutdown ();

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

  void cancel (const RTScheduling::Current::IdType & guid) override;

  CORBA::PolicyList * scheduling_policies () override;

  void scheduling_policies (const CORBA::PolicyList & scheduling_policies) override;

  CORBA::PolicyList * poa_policies () override;

  char * scheduling_discipline_name () override;

  RTScheduling::ResourceManager_ptr create_resource_manager (
    const char * name,
    CORBA::Policy_ptr scheduling_parameter) override;

  void set_scheduling_parameter (PortableServer::Servant & resource,
                                 const char * name,
                                 CORBA::Policy_ptr scheduling_parameter) override;

 private:
  CORBA::ORB_var orb_;
  IOP::Codec_var codec_;
  RTScheduling::Current_var current_;
  Kokyu::DSRT_Dispatcher_Factory<MUF_Scheduler_Traits>::DSRT_Dispatcher_Auto_Ptr
    kokyu_dispatcher_;
  Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type_;
  int ace_sched_policy_;
  int ace_sched_scope_;
};

#endif //MUF_SCHEDULER_H
