#include "Scheduler.h"
#include "tao/ORB.h"
#include "tao/RTScheduling/Request_Interceptor.h"
#include <cstring>
#include <memory>

TAO_Scheduler::TAO_Scheduler (CORBA::ORB_ptr orb)
{
  CORBA::Object_var current_obj =
    orb->resolve_initial_references ("RTScheduler_Current");

  current_ = RTScheduling::Current::_narrow (current_obj.in ());
}

CORBA::PolicyList*
TAO_Scheduler::scheduling_policies ()
{
  return nullptr;
}

void
TAO_Scheduler::scheduling_policies (CORBA::PolicyList const &)
{
}

CORBA::PolicyList*
TAO_Scheduler::poa_policies ()
{
  return nullptr;
}

char *
TAO_Scheduler::scheduling_discipline_name ()
{
  return nullptr;
}

RTScheduling::ResourceManager_ptr
TAO_Scheduler::create_resource_manager (char const *,
                                        CORBA::Policy_ptr)
{
  return nullptr;
}

void
TAO_Scheduler::set_scheduling_parameter (PortableServer::Servant &,
                                         char const *,
                                         CORBA::Policy_ptr)
{
}

void
TAO_Scheduler::begin_new_scheduling_segment (RTScheduling::Current::IdType const &,
                                             char const *,
                                             CORBA::Policy_ptr,
                                             CORBA::Policy_ptr)
{
}

void
TAO_Scheduler::begin_nested_scheduling_segment (RTScheduling::Current::IdType const &,
                                                char const *,
                                                CORBA::Policy_ptr,
                                                CORBA::Policy_ptr)
{
}

void
TAO_Scheduler::update_scheduling_segment (RTScheduling::Current::IdType const &,
                                          char const *,
                                          CORBA::Policy_ptr,
                                          CORBA::Policy_ptr)
{
}

void
TAO_Scheduler::end_scheduling_segment (RTScheduling::Current::IdType const &,
                                       char const *)
{
}

void
TAO_Scheduler::end_nested_scheduling_segment (RTScheduling::Current::IdType const &,
                                              char const *,
                                              CORBA::Policy_ptr)
{
}


void
TAO_Scheduler::send_request (PortableInterceptor::ClientRequestInfo_ptr request_info)
{
  IOP::ServiceContext srv_con;
  srv_con.context_id = Client_Interceptor::SchedulingInfo;
  srv_con.context_data.length (sizeof (size_t));
  RTScheduling::Current::IdType_var id = this->current_->id ();
  std::memcpy (srv_con.context_data.get_buffer (),
               id->get_buffer (),
               sizeof (size_t));
  request_info->add_request_service_context (srv_con,
                                             0);
}

void
TAO_Scheduler::receive_request (PortableInterceptor::ServerRequestInfo_ptr request_info,
                                RTScheduling::Current::IdType_out guid_out,
                                CORBA::String_out /*name*/,
                                CORBA::Policy_out /*sched_param*/,
                                CORBA::Policy_out /*implicit_sched_param*/)
{
  IOP::ServiceContext_var serv_cxt;

  try
    {
      serv_cxt = request_info->get_request_service_context (Server_Interceptor::SchedulingInfo);

      size_t gu_id {};
      std::memcpy (&gu_id,
                   serv_cxt->context_data.get_buffer (),
                   serv_cxt->context_data.length ());

      ACE_DEBUG ((LM_DEBUG,
                  "The Guid is %d\n",
                  gu_id));

      std::unique_ptr<RTScheduling::Current::IdType> guid =
        std::make_unique<RTScheduling::Current::IdType> ();

      guid->length (sizeof (size_t));
      std::memcpy (guid->get_buffer (),
                   serv_cxt->context_data.get_buffer (),
                   sizeof (size_t));

      guid_out = guid.release ();
    }
  catch (CORBA::Exception const &)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Invalid Service Context\n"));
    }
}

void
TAO_Scheduler::send_reply (PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
TAO_Scheduler::send_exception (PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
TAO_Scheduler::send_other (PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
TAO_Scheduler::send_poll (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_Scheduler::receive_reply (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_Scheduler::receive_exception (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_Scheduler::receive_other (PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_Scheduler::cancel (RTScheduling::Current::IdType const &)
{
}
