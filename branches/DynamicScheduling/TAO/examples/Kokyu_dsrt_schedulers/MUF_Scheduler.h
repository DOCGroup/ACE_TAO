//$Id$

#ifndef MUF_SCHEDULER_H
#define MUF_SCHEDULER_H

#include "tao/RTScheduling/RTSchedulerC.h"
#include "MUF_SchedulingC.h"
#include "Kokyu_dsrt.h"
#include "Kokyu_dsrt_schedulers_export.h"

class Kokyu_DSRT_Schedulers_Export MUF_Sched_Param_Policy:
public MUF_Scheduling::SchedulingParameterPolicy,
       public TAO_Local_RefCounted_Object
{
 public:

    MUF_Scheduling::SchedulingParameter value (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    void value (const MUF_Scheduling::SchedulingParameter & value
        ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

 private:
    MUF_Scheduling::SchedulingParameter value_;
};

class Kokyu_DSRT_Schedulers_Export MUF_Scheduler:
public MUF_Scheduling::Scheduler,
public TAO_Local_RefCounted_Object
{
 public:

  MUF_Scheduler (CORBA::ORB_ptr orb);

  ~MUF_Scheduler (void);


  virtual MUF_Scheduling::SchedulingParameterPolicy_ptr
    create_scheduling_parameter (const MUF_Scheduling::SchedulingParameter & value
                                 ACE_ENV_ARG_DECL_WITH_DEFAULTS
                                 )
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (void);

  virtual void begin_new_scheduling_segment (const RTScheduling::Current::IdType & guid,
                                             const char * name,
                                             CORBA::Policy_ptr sched_param,
                                             CORBA::Policy_ptr implicit_sched_param
                                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE));

  virtual void begin_nested_scheduling_segment (const RTScheduling::Current::IdType & guid,
                                                const char * name,
                                                CORBA::Policy_ptr sched_param,
                                                CORBA::Policy_ptr implicit_sched_param
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE));

  virtual void update_scheduling_segment (const RTScheduling::Current::IdType & guid,
                                          const char * name,
                                          CORBA::Policy_ptr sched_param,
                                          CORBA::Policy_ptr implicit_sched_param
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE));

  virtual void end_scheduling_segment (const RTScheduling::Current::IdType & guid,
                                       const char * name
                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void end_nested_scheduling_segment (const RTScheduling::Current::IdType & guid,
                                              const char * name,
                                              CORBA::Policy_ptr outer_sched_param
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                RTScheduling::Current::IdType_out guid,
                                CORBA::String_out name,
                                CORBA::Policy_out sched_param,
                                CORBA::Policy_out implicit_sched_param
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr ri
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void cancel (const RTScheduling::Current::IdType & guid
                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyList * scheduling_policies (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void scheduling_policies (const CORBA::PolicyList & scheduling_policies
                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyList * poa_policies (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * scheduling_discipline_name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RTScheduling::ResourceManager_ptr create_resource_manager (const char * name,
                                                                     CORBA::Policy_ptr scheduling_parameter
                                                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_scheduling_parameter (PortableServer::Servant & resource,
                                         const char * name,
                                         CORBA::Policy_ptr scheduling_parameter
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  struct MUF_Scheduler_Traits
  {
    typedef RTScheduling::Current::IdType Guid_t;

    struct _QoSDescriptor_t
    {
      typedef long Criticality_t;
      typedef TimeBase::TimeT Time_t;
      class _Now
      {
      public:
        Time_t operator () ()
        {
          ACE_Time_Value time = ACE_OS::gettimeofday ();
          return 10*time.usec () + 10000000*time.sec ();
        }
      };

      typedef _Now Now;

      Criticality_t criticality_;
      Time_t deadline_;
      Time_t exec_time_;
    };

    typedef _QoSDescriptor_t QoSDescriptor_t;

    typedef Kokyu::MUF_Comparator<QoSDescriptor_t> QoSComparator_t;

    class _Guid_Hash
    {
    public:
      u_long operator () (const Guid_t& id)
      {
        return ACE::hash_pjw ((const char *) id.get_buffer (),
                              id.length ());
      }
    };
    typedef _Guid_Hash Guid_Hash;
  };

 private:
  CORBA::ORB_var orb_;
  IOP::Codec_var codec_;
  RTScheduling::Current_var current_;
  Kokyu::DSRT_Dispatcher_Factory<MUF_Scheduler_Traits>::DSRT_Dispatcher_Auto_Ptr
    kokyu_dispatcher_;
};

#endif //MUF_SCHEDULER_H
