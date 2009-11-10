//$Id$

#ifndef MIF_SCHEDULER_H
#define MIF_SCHEDULER_H

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/CodecFactory/CodecFactory.h"
#include "MIF_SchedulingC.h"
#include "tao/LocalObject.h"
#include "Kokyu_dsrt.h"
#include "Kokyu_dsrt_schedulers_export.h"

struct MIF_Scheduler_Traits
{
  typedef RTScheduling::Current::IdType Guid_t;

  struct _QoSDescriptor_t
  {
    typedef long Importance_t;
    Importance_t importance_;
  };

  typedef _QoSDescriptor_t QoSDescriptor_t;

  typedef Kokyu::MIF_Comparator<QoSDescriptor_t> QoSComparator_t;

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

class Kokyu_DSRT_Schedulers_Export MIF_Sched_Param_Policy:
public MIF_Scheduling::SchedulingParameterPolicy,
       public ::CORBA::LocalObject
{
 public:
     MIF_Sched_Param_Policy ();
     MIF_Sched_Param_Policy (const MIF_Sched_Param_Policy &rhs);

    MIF_Scheduling::SchedulingParameter value (void);

    void value (const MIF_Scheduling::SchedulingParameter & value);

     virtual CORBA::Policy_ptr copy ();

     virtual void destroy ();

 private:
    MIF_Scheduling::SchedulingParameter value_;
};

class Kokyu_DSRT_Schedulers_Export MIF_Scheduler:
public MIF_Scheduling::Scheduler,
public ::CORBA::LocalObject
{
 public:

  MIF_Scheduler (CORBA::ORB_ptr orb,
                 Kokyu::DSRT_Dispatcher_Impl_t,
                 int ace_sched_policy,
                 int ace_sched_scope);

  ~MIF_Scheduler (void);


  virtual MIF_Scheduling::SchedulingParameterPolicy_ptr
    create_scheduling_parameter (const MIF_Scheduling::SchedulingParameter & value
                                 );

  void shutdown (void);

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

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr ri);

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
  CORBA::ORB_var orb_;
  IOP::Codec_var codec_;
  RTScheduling::Current_var current_;
  Kokyu::DSRT_Dispatcher_Factory<MIF_Scheduler_Traits>::DSRT_Dispatcher_Auto_Ptr
  kokyu_dispatcher_;
  Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type_;
  int ace_sched_policy_;
  int ace_sched_scope_;
};

#endif //MIF_SCHEDULER_H
