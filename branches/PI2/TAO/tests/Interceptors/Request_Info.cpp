// $Id$

#include "./Request_Info.h"

//****************************************************************               

Request_Info::Request_Info (char * operation, 
                          Dynamic::ParameterList * arguments,
                          Dynamic::ExceptionList * exceptions,
                          Dynamic::ContextList * contexts,
                          Dynamic::RequestContext * operation_context,
                          CORBA::Object_ptr forward_reference,
                          CORBA::Environment &)
  : operation_ (operation),
    parameter_list_ (arguments),
    exception_list_ (exceptions),
    context_list_ (contexts),
    request_context_ (operation_context),
    forward_reference_ (CORBA::Object::_duplicate (forward_reference))
{
}

char * 
Request_Info::operation (CORBA::Environment &)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->operation_;
}

Dynamic::ParameterList * 
Request_Info::arguments (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
 return this->parameter_list_._retn ();
}

Dynamic::ExceptionList * 
Request_Info::exceptions (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->exception_list_._retn ();
}

Dynamic::ContextList * 
Request_Info::contexts (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return this->context_list_._retn ();
}

Dynamic::RequestContext * 
Request_Info::operation_context (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return this->request_context_._retn ();
}

CORBA::Any * 
Request_Info::result (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Boolean 
Request_Info::response_expected (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

Messaging::SyncScope 
Request_Info::sync_scope (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

PortableInterceptor::ReplyStatus 
Request_Info::reply_status (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Object_ptr 
Request_Info::forward_reference (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Any * 
Request_Info::get_slot (PortableInterceptor::SlotId id,
                       CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  return 0;
}

IOP::ServiceContext * 
Request_Info::get_request_service_context (IOP::ServiceId id,
                              CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong size = 0; size < this->service_context_list_->length (); ++size)
    if (this->service_context_list_[size].context_id == id)
      {
         return &(this->service_context_list_[size]);
        
      }
}

IOP::ServiceContext * 
Request_Info::get_reply_service_context (IOP::ServiceId id,
                            CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  for (CORBA::ULong size = 0; size < this->service_context_list_->length (); ++size)
    if (this->service_context_list_[size].context_id == id)
      {
        // Used <_retn> becos then it will get safely destroyed at the
        // client side when the service context gets back.
        return &this->service_context_list_[size];
        
      }
//****************************************************************               

ClientRequest_Info:: ClientRequest_Info (CORBA_Object * target,
                                       CORBA_Object * effective_target,
                                       IOP::ServiceContextList &service_context_list,               
                                       char * operation, 
                                       Dynamic::ParameterList * arguments,
                                       Dynamic::ExceptionList * exceptions,
                                       Dynamic::ContextList * contexts,
                                       Dynamic::RequestContext * operation_context,
                                       CORBA::Object_ptr forward_reference,
                                       CORBA::Environment &)
  : target_ (CORBA::Object::_duplicate (target)), service_context_list_ (service_context_list)
    Request_Info (operation, arguments, exceptions, contexts, operation_context, forward_reference)
{
}
  
CORBA::Object_ptr 
ClientRequest_Info::target (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->target_._retn ();
}

CORBA::Object_ptr  ClientRequest_Info::effective_target (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

IOP::TaggedProfile *  
ClientRequest_Info::effective_profile (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Any *  
ClientRequest_Info::received_exception (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}
  
char *  
ClientRequest_Info::received_exception_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

IOP::TaggedComponent *  
ClientRequest_Info::get_effective_component (IOP::ComponentId id,
                                            CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}
 
CORBA::Policy_ptr  
ClientRequest_Info::get_request_policy (CORBA::PolicyType type,
                                       CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void  
ClientRequest_Info::add_request_service_context (const IOP::ServiceContext & service_context,
                                                CORBA::Boolean replace,
                                                CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException));   
{
  // Need to take care of the replace semantics, see spec
  CORBA::ULong length = this->service_context_list_->length ();
  this->service_context_list_->length (length + 1);

  // Hey the service context is passe dby ref, do i haveto worry?
  service_context_list_[length] = service_context;
}
//****************************************************************               

 
ServerRequest_Info::ServerRequest_Info(PortableInterceptor::OctetSeq *object_id,
                                     PortableInterceptor::OctetSeq *adapter_id,
                                     char * operation, 
                                     Dynamic::ParameterList * arguments,
                                     Dynamic::ExceptionList * exceptions,
                                     Dynamic::ContextList * contexts,
                                     Dynamic::RequestContext * operation_context,
                                     CORBA::Object_ptr forward_reference,
                                     CORBA::Environment &)
  : object_id_ (object_id),
    adapter_id_ (adapter_id),
    Request_Info (operation, arguments, exceptions, contexts, operation_context, forward_reference)
{
}

CORBA::Any * 
ServerRequest_Info::sending_exception (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}
  
PortableInterceptor::OctetSeq * 
ServerRequest_Info::object_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 
}

PortableInterceptor::OctetSeq * 
ServerRequest_Info::adapter_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

char * 
ServerRequest_Info::target_most_derived_interface (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::Policy_ptr 
ServerRequest_Info::get_server_policy (CORBA::PolicyType type,
                                      CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{ 
  return 0;
}

void 
ServerRequest_Info::set_slot (PortableInterceptor::SlotId id,
                             const CORBA::Any & data,
                             CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
}


CORBA::Boolean 
ServerRequest_Info::target_is_a (const char * id,
                                CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void 
ServerRequest_Info::add_reply_service_context (const IOP::ServiceContext & service_context,
                                              CORBA::Boolean replace,
                                              CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
