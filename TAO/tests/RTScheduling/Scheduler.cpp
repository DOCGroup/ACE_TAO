//$Id$

#include "Scheduler.h"
#include "tao/ORB.h"
#include "ace/Atomic_Op.h"
#include "tao/RTScheduling/Request_Interceptor.h"

TAO_Scheduler::TAO_Scheduler (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL)
{
  CORBA::Object_var current_obj =
    orb->resolve_initial_references ("RTScheduler_Current"
                                     ACE_ENV_ARG_PARAMETER);

  current_ = RTScheduling::Current::_narrow (current_obj.in ()
                                             ACE_ENV_ARG_PARAMETER);
}

TAO_Scheduler::~TAO_Scheduler (void)
{
}

CORBA::PolicyList*
TAO_Scheduler::scheduling_policies (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void
TAO_Scheduler::scheduling_policies (const CORBA::PolicyList &
                                    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyList*
TAO_Scheduler::poa_policies (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

char *
TAO_Scheduler::scheduling_discipline_name (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

RTScheduling::ResourceManager_ptr
TAO_Scheduler::create_resource_manager (const char *,
                                        CORBA::Policy_ptr
                                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void
TAO_Scheduler::set_scheduling_parameter (PortableServer::Servant &,
                                         const char *,
                                         CORBA::Policy_ptr
                                         ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_Scheduler::begin_new_scheduling_segment (const RTScheduling::Current::IdType &,
                                             const char *,
                                             CORBA::Policy_ptr,
                                             CORBA::Policy_ptr
                                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
}

void
TAO_Scheduler::begin_nested_scheduling_segment (const RTScheduling::Current::IdType &,
                                                const char *,
                                                CORBA::Policy_ptr,
                                                CORBA::Policy_ptr
                                                ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
}

void
TAO_Scheduler::update_scheduling_segment (const RTScheduling::Current::IdType &,
                                          const char *,
                                          CORBA::Policy_ptr,
                                          CORBA::Policy_ptr
                                          ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   RTScheduling::Current::UNSUPPORTED_SCHEDULING_DISCIPLINE))
{
}

void
TAO_Scheduler::end_scheduling_segment (const RTScheduling::Current::IdType &,
                                       const char *
                                       ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_Scheduler::end_nested_scheduling_segment (const RTScheduling::Current::IdType &,
                                              const char *,
                                              CORBA::Policy_ptr
                                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}


void
TAO_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr request_info
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  IOP::ServiceContext* srv_con = new IOP::ServiceContext;
  srv_con->context_id = Client_Interceptor::SchedulingInfo;
  srv_con->context_data.length (sizeof (size_t));
  ACE_OS::memcpy (srv_con->context_data.get_buffer (),
                  current_->id ()->get_buffer (),
                  sizeof (size_t));
  request_info->add_request_service_context (*srv_con,
                                             0);
}

void
TAO_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr request_info,
                                RTScheduling::Current::IdType_out guid_out,
                                CORBA::String_out /*name*/,
                                CORBA::Policy_out /*sched_param*/,
                                CORBA::Policy_out /*implicit_sched_param*/
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  IOP::ServiceContext_var serv_cxt;

  ACE_TRY
    {
      serv_cxt = request_info->get_request_service_context (Server_Interceptor::SchedulingInfo
                                                            ACE_ENV_ARG_PARAMETER);

      size_t gu_id;
      ACE_OS::memcpy (&gu_id,
                      serv_cxt->context_data.get_buffer (),
                      serv_cxt->context_data.length ());

      ACE_DEBUG ((LM_DEBUG,
                  "The Guid is %d\n",
                  gu_id));

      RTScheduling::Current::IdType* guid;
      ACE_NEW (guid,
               RTScheduling::Current::IdType);

      guid->length (sizeof (size_t));
      ACE_OS::memcpy (guid->get_buffer (),
                      serv_cxt->context_data.get_buffer (),
                      sizeof (size_t));

      guid_out.ptr () = guid;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Invalid Service Context\n"));
    }
  ACE_ENDTRY;
}

void
TAO_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr
                               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO_Scheduler::send_other (PortableInterceptor::ServerRequestInfo_ptr
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO_Scheduler::send_poll (PortableInterceptor::ClientRequestInfo_ptr
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO_Scheduler::receive_reply (PortableInterceptor::ClientRequestInfo_ptr
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_Scheduler::receive_exception (PortableInterceptor::ClientRequestInfo_ptr
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO_Scheduler::receive_other (PortableInterceptor::ClientRequestInfo_ptr
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
TAO_Scheduler::cancel (const RTScheduling::Current::IdType &
                       ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
