//$Id$

#ifndef FIXED_PRIORITY_SCHEDULER_H
#define FIXED_PRIORITY_SCHEDULER_H

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/CodecFactory/CodecFactory.h"
#include "tao/LocalObject.h"
#include "FP_SchedulingC.h"
#include "Kokyu_dsrt.h"
#include "Kokyu_dsrt_schedulers_export.h"

struct FP_Scheduler_Traits
{
  typedef RTScheduling::Current::IdType Guid_t;

  struct _QoSDescriptor_t
  {
    typedef long Priority_t;
    Priority_t priority_;
  };

  typedef _QoSDescriptor_t QoSDescriptor_t;

  typedef Kokyu::Fixed_Priority_Comparator<QoSDescriptor_t> QoSComparator_t;

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

class Kokyu_DSRT_Schedulers_Export FP_Segment_Sched_Param_Policy:
public FP_Scheduling::SegmentSchedulingParameterPolicy,
       public ::CORBA::LocalObject
{
 public:
     FP_Segment_Sched_Param_Policy ();
     FP_Segment_Sched_Param_Policy (const FP_Segment_Sched_Param_Policy &rhs);

     FP_Scheduling::SegmentSchedulingParameter value (
      );

     void value (
        const FP_Scheduling::SegmentSchedulingParameter & value
      );

     virtual CORBA::Policy_ptr copy ();

     virtual void destroy ();

 private:
  FP_Scheduling::SegmentSchedulingParameter value_;
};

class Kokyu_DSRT_Schedulers_Export Fixed_Priority_Scheduler:
public FP_Scheduling::FP_Scheduler,
public ::CORBA::LocalObject
{
 public:

  Fixed_Priority_Scheduler (CORBA::ORB_ptr orb,
                 Kokyu::DSRT_Dispatcher_Impl_t,
                 int ace_sched_policy,
                 int ace_sched_scope);

  ~Fixed_Priority_Scheduler (void);

  void shutdown (void);

  virtual FP_Scheduling::SegmentSchedulingParameterPolicy_ptr
    create_segment_scheduling_parameter (
        const FP_Scheduling::SegmentSchedulingParameter & value
      );

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

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri);

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
  Kokyu::DSRT_Dispatcher_Factory<FP_Scheduler_Traits>::DSRT_Dispatcher_Auto_Ptr
  kokyu_dispatcher_;
  Kokyu::DSRT_Dispatcher_Impl_t disp_impl_type_;
  int ace_sched_policy_;
  int ace_sched_scope_;
};

#endif //FIXED_PRIORITY_SCHEDULER_H
