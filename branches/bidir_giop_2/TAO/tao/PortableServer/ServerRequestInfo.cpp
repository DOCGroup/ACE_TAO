// -*- C++ -*-
//
// $Id$

#include "ServerRequestInfo.h"

ACE_RCSID (PortableServer, ServerRequestInfo, "$Id$")

#if (TAO_HAS_INTERCEPTORS == 1)

TAO_ServerRequestInfo::TAO_ServerRequestInfo (
      const char * operation,
      IOP::ServiceContextList &service_context_list)
  : operation_ (operation),
    service_context_list_ (service_context_list)
{
}

CORBA::ULong
TAO_ServerRequestInfo::request_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->request_id_;
}

char *
TAO_ServerRequestInfo::operation (CORBA::Environment &)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->operation_);
}

Dynamic::ParameterList *
TAO_ServerRequestInfo::arguments (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->parameter_list_;
}

Dynamic::ExceptionList *
TAO_ServerRequestInfo::exceptions (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return &this->exception_list_;
}

Dynamic::ContextList *
TAO_ServerRequestInfo::contexts (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->context_list_;
}

Dynamic::RequestContext *
TAO_ServerRequestInfo::operation_context (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return &this->request_context_;
}

CORBA::Any *
TAO_ServerRequestInfo::result (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Boolean
TAO_ServerRequestInfo::response_expected (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

# if TAO_HAS_CORBA_MESSAGING == 1
Messaging::SyncScope
TAO_ServerRequestInfo::sync_scope (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}
#endif  /* TAO_HAS_CORBA_MESSAGING */

PortableInterceptor::ReplyStatus
TAO_ServerRequestInfo::reply_status (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Object_ptr
TAO_ServerRequestInfo::forward_reference (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::Any *
TAO_ServerRequestInfo::get_slot (PortableInterceptor::SlotId,
                                 CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
  return 0;
}

IOP::ServiceContext *
TAO_ServerRequestInfo::get_request_service_context (IOP::ServiceId id,
                                                    CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong size = 0;
       size < this->service_context_list_.length ();
       ++size)
    {
      if (this->service_context_list_[size].context_id == id)
        return &(this->service_context_list_[size]);
    }

  return 0;
}

IOP::ServiceContext *
TAO_ServerRequestInfo::get_reply_service_context (IOP::ServiceId id,
                                                  CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for (CORBA::ULong size = 0;
       size < this->service_context_list_.length ();
       ++size)
    {
      if (this->service_context_list_[size].context_id == id)
        return &this->service_context_list_[size];
    }

  return 0;
}


// Use at own risk. There is no way currently of extracting an
// exception from an Any. This method is in place just to be compliant
// with the spec.
CORBA::Any *
TAO_ServerRequestInfo::sending_exception (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The spec says that if it is a user exception which cant be inserted
  // then the UNKNOWN exception needs to be thrown with minor code TBD_U.
  this->any_exception_ <<= *this->caught_exception_;
  return &this->any_exception_;
}

// = TAO specific method done since there currently is no simple way
// to extract exceptions from an Any
CORBA::Exception *
TAO_ServerRequestInfo::_sending_exception (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->caught_exception_;
}

CORBA::OctetSeq *
TAO_ServerRequestInfo::object_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

CORBA::OctetSeq *
TAO_ServerRequestInfo::adapter_id (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

char *
TAO_ServerRequestInfo::target_most_derived_interface (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
 return 0;
}

CORBA::Policy_ptr
TAO_ServerRequestInfo::get_server_policy (CORBA::PolicyType,
                                          CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void
TAO_ServerRequestInfo::set_slot (PortableInterceptor::SlotId,
                                 const CORBA::Any &,
                                 CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::InvalidSlot))
{
}

CORBA::Boolean
TAO_ServerRequestInfo::target_is_a (const char *,
                                    CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void
TAO_ServerRequestInfo::add_reply_service_context (
    const IOP::ServiceContext & service_context,
    CORBA::Boolean,
    CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong length = this->service_context_list_.length ();
  this->service_context_list_.length (length + 1);
  service_context_list_[length] = service_context;
}

void
TAO_ServerRequestInfo::exception (CORBA::Exception *exception)
{
  this->caught_exception_ = exception;
}

void
TAO_ServerRequestInfo::request_id (CORBA::ULong request_id)
{
  this->request_id_ = request_id;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
