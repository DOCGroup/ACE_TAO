//$Id$

#ifndef FIXED_PRIORITY_SCHEDULER_H
#define FIXED_PRIORITY_SCHEDULER_H

#include "tao/RTScheduling/RTScheduler.h"
#include "../FP_SchedulingC.h"
#include "tao/LocalObject.h"

class Segment_Sched_Param_Policy:
public FP_Scheduling::SegmentSchedulingParameterPolicy,
       public TAO_Local_RefCounted_Object
{
 public:
  virtual RTCORBA::Priority value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void value (RTCORBA::Priority value
                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

 private:
  RTCORBA::Priority value_;
};

class Fixed_Priority_Scheduler:
public FP_Scheduling::FP_Scheduler,
public TAO_Local_RefCounted_Object
{
 public:

  Fixed_Priority_Scheduler (CORBA::ORB_ptr orb);

  ~Fixed_Priority_Scheduler (void);

  virtual FP_Scheduling::SegmentSchedulingParameterPolicy_ptr
    create_segment_scheduling_parameter (RTCORBA::Priority segment_priority
                                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

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

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri
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
  RTScheduling::Current_var current_;
  RTCORBA::PriorityMapping* pm_;

};

#endif //FIXED_PRIORITY_SCHEDULER_H
