// $Id$

#include "Request_Info.h"
#include "tao/corba.h"
//****************************************************************               

/*Request_Info::Request_Info (CORBA::ULong request_id,
                            char * operation, 
                            Dynamic::ParameterList * arguments,
                            Dynamic::ExceptionList * exceptions,
                            Dynamic::ContextList * contexts,
                            Dynamic::RequestContext * operation_context,
                            CORBA::Object_ptr forward_reference,
                            CORBA::Environment &)
  : request_id_ (request_id),
    operation_ (operation),
    parameter_list_ (arguments),
    exception_list_ (exceptions),
    context_list_ (contexts),
    request_context_ (operation_context),
    forward_reference_ (CORBA::Object::_duplicate (forward_reference))
{
}
*/


CORBA::ULong 
ClientRequest_Info::request_id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

char * 
ClientRequest_Info::operation (CORBA::Environment &)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->operation_;
}

Dynamic::ParameterList * 
ClientRequest_Info::arguments (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
 return this->parameter_list_._retn ();
}

Dynamic::ExceptionList * 
ClientRequest_Info::exceptions (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->exception_list_._retn ();
}

Dynamic::ContextList * 
ClientRequest_Info::contexts (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return this->context_list_._retn ();
}

Dynamic::RequestContext * 
ClientRequest_Info::operation_context (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return this->request_context_._retn ();
}

CORBA::Any * 
ClientRequest_Info::result (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Boolean 
ClientRequest_Info::response_expected (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

Messaging::SyncScope 
ClientRequest_Info::sync_scope (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

PortableInterceptor::ReplyStatus 
ClientRequest_Info::reply_status (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Object_ptr 
ClientRequest_Info::forward_reference (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Any * 
ClientRequest_Info::get_slot (PortableInterceptor::SlotId id,
                       CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  return 0;
}

IOP::ServiceContext * 
ClientRequest_Info::get_request_service_context (IOP::ServiceId id,
                              CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong size = 0; size < this->service_context_list_.length (); ++size)
    if (this->service_context_list_[size].context_id == id)
      {
         return &(this->service_context_list_[size]);
        
      }
}

IOP::ServiceContext * 
ClientRequest_Info::get_reply_service_context (IOP::ServiceId id,
                            CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  for (CORBA::ULong size = 0; size < this->service_context_list_.length (); ++size)
    if (this->service_context_list_[size].context_id == id)
      {
        // Used <_retn> becos then it will get safely destroyed at the
        // client side when the service context gets back.
        return &this->service_context_list_[size];
        
      }
}

/*ClientRequest_Info:: ClientRequest_Info (CORBA::ULong request_id,
                                         char * operation, 
                                         Dynamic::ParameterList * arguments,
                                         Dynamic::ExceptionList * exceptions,
                                         Dynamic::ContextList * contexts,
                                         Dynamic::RequestContext * operation_context,
                                         CORBA::Object_ptr forward_reference,
                                         CORBA_Object_ptr target,
                                         CORBA_Object_ptr effective_target,
                                         IOP::ServiceContextList_ptr &service_context_list,               
                                         CORBA::Environment &)
  :    Request_Info (request_id, operation, arguments, exceptions, contexts, operation_context, forward_reference),
       target_ (CORBA::Object::_duplicate (target))
 
{
  this->service_context_list_ = service_context_list;
}
*/

ClientRequest_Info::ClientRequest_Info (CORBA::Environment &,
                                        char * operation,
                                        IOP::ServiceContextList &service_context_list ,               
                                        CORBA::Object * target)
  : operation_ (operation), 
    service_context_list_ (service_context_list),
    target_ (target)
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
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  // Need to take care of the replace semantics, see spec
  CORBA::ULong length = this->service_context_list_.length ();
  this->service_context_list_.length (length + 1);

  // Hey the service context is passe dby ref, do i haveto worry?
  service_context_list_[length] = service_context;
}
//****************************************************************               

CORBA::ULong 
ServerRequest_Info::request_id (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

char * 
ServerRequest_Info::operation (CORBA::Environment &)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->operation_;
}

Dynamic::ParameterList * 
ServerRequest_Info::arguments (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
 return this->parameter_list_._retn ();
}

Dynamic::ExceptionList * 
ServerRequest_Info::exceptions (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->exception_list_._retn ();
}

Dynamic::ContextList * 
ServerRequest_Info::contexts (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return this->context_list_._retn ();
}

Dynamic::RequestContext * 
ServerRequest_Info::operation_context (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return this->request_context_._retn ();
}

CORBA::Any * 
ServerRequest_Info::result (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Boolean 
ServerRequest_Info::response_expected (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

Messaging::SyncScope 
ServerRequest_Info::sync_scope (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

PortableInterceptor::ReplyStatus 
ServerRequest_Info::reply_status (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Object_ptr 
ServerRequest_Info::forward_reference (CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Any * 
ServerRequest_Info::get_slot (PortableInterceptor::SlotId id,
                       CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  return 0;
}

IOP::ServiceContext * 
ServerRequest_Info::get_request_service_context (IOP::ServiceId id,
                              CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong size = 0; size < this->service_context_list_.length (); ++size)
    if (this->service_context_list_[size].context_id == id)
      {
         return &(this->service_context_list_[size]);
        
      }
}

IOP::ServiceContext * 
ServerRequest_Info::get_reply_service_context (IOP::ServiceId id,
                            CORBA::Environment &)  
  ACE_THROW_SPEC ((CORBA::SystemException)) 
{
  for (CORBA::ULong size = 0; size < this->service_context_list_.length (); ++size)
    if (this->service_context_list_[size].context_id == id)
      {
        // Used <_retn> becos then it will get safely destroyed at the
        // client side when the service context gets back.
        return &this->service_context_list_[size];
        
      }
}

/* 
ServerRequest_Info::ServerRequest_Info(CORBA::ULong request_id,
                                       char * operation, 
                                       Dynamic::ParameterList * arguments,
                                       Dynamic::ExceptionList * exceptions,
                                       Dynamic::ContextList * contexts,
                                       Dynamic::RequestContext * operation_context,
                                       CORBA::Object_ptr forward_reference,
                                       PortableInterceptor::OctetSeq *object_id,
                                       PortableInterceptor::OctetSeq *adapter_id,
                                       CORBA::Environment &)
  : Request_Info (request_id, operation, arguments, exceptions, contexts, operation_context, forward_reference),
    object_id_ (object_id),
    adapter_id_ (adapter_id)
{
}
*/
ServerRequest_Info::ServerRequest_Info (char * operation,
                                        IOP::ServiceContextList &service_context_list ,               
                                        CORBA::Environment &)
  : operation_ (operation), 
    service_context_list_ (service_context_list)
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
  return 0; 
}

PortableInterceptor::OctetSeq * 
ServerRequest_Info::adapter_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

char * 
ServerRequest_Info::target_most_derived_interface (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
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
  // Need to take care of the replace semantics, see spec
  CORBA::ULong length = this->service_context_list_.length ();
  this->service_context_list_.length (length + 1);

  // Hey the service context is passe dby ref, do i haveto worry?
  service_context_list_[length] = service_context;

}
