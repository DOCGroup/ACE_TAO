// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tao/
//
// = FILENAME
//     Request_Info.h
//
// = DESCRIPTION
//     This is the implementation of the RequestInfo interface of the
//   Portable Interceptor specification.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//=========================================================================


#ifndef REQUEST_INFO_H
#define REQUEST_INFO_H

//#include "tao/corbafwd.h"
#include "tao/PortableInterceptorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


//****************************************************************               

class TAO_Export TAO_ClientRequest_Info 
: public virtual PortableInterceptor::ClientRequestInfo
{
 public:
 
  /*ClientRequest_Info (CORBA::ULong request_id,
                      char * operation, 
                      Dynamic::ParameterList * arguments,
                      Dynamic::ExceptionList * exceptions,
                      Dynamic::ContextList * contexts,
                      Dynamic::RequestContext * operation_context,
                      CORBA::Object_ptr forward_reference,
                      CORBA::Object * target,
                      CORBA::Object * effective_target,
                      IOP::ServiceContextList_ptr &service_context_list ,               
                      CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ());*/

  TAO_ClientRequest_Info (const char * operation,
                      IOP::ServiceContextList &service_context_list,               
                      CORBA::Object * target,
                      CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ());

  virtual CORBA::ULong request_id (CORBA::Environment &ACE_TRY_ENV = 
                                   TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * operation (CORBA::Environment &ACE_TRY_ENV = 
                            TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ParameterList * arguments (CORBA::Environment &ACE_TRY_ENV = 
                                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ExceptionList * exceptions (CORBA::Environment &ACE_TRY_ENV = 
                                               TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ContextList * contexts (CORBA::Environment &ACE_TRY_ENV = 
                                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::RequestContext * operation_context (CORBA::Environment &ACE_TRY_ENV = 
                                                       TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Any * result (CORBA::Environment &ACE_TRY_ENV = 
                               TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean response_expected (CORBA::Environment &ACE_TRY_ENV = 
                                            TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

  virtual Messaging::SyncScope sync_scope (CORBA::Environment &ACE_TRY_ENV = 
                                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual PortableInterceptor::ReplyStatus reply_status (CORBA::Environment &ACE_TRY_ENV = 
                                                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Object_ptr forward_reference (CORBA::Environment &ACE_TRY_ENV = 
                                               TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

  // Probably the following methods shoudl be delegated to teh POICurrentx
  virtual CORBA::Any * get_slot (PortableInterceptor::SlotId id,
                                 CORBA::Environment &ACE_TRY_ENV = 
                                 TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));

  virtual IOP::ServiceContext * get_request_service_context (IOP::ServiceId id,
                                                           CORBA::Environment &ACE_TRY_ENV = 
                                                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::ServiceContext * get_reply_service_context (IOP::ServiceId id,
                                                           CORBA::Environment &ACE_TRY_ENV = 
                                                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Object_ptr target (CORBA::Environment &ACE_TRY_ENV = 
                                    TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Object_ptr effective_target (CORBA::Environment &ACE_TRY_ENV = 
                                              TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual IOP::TaggedProfile * effective_profile (CORBA::Environment &ACE_TRY_ENV = 
                                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Any * received_exception (CORBA::Environment &ACE_TRY_ENV = 
                                           TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual char * received_exception_id (CORBA::Environment &ACE_TRY_ENV = 
                                        TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));

   virtual IOP::TaggedComponent * get_effective_component (IOP::ComponentId id,
                                                           CORBA::Environment &ACE_TRY_ENV = 
                                                           TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));
 
   virtual CORBA::Policy_ptr get_request_policy (CORBA::PolicyType type,
                                                 CORBA::Environment &ACE_TRY_ENV = 
                                                 TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));

   virtual void add_request_service_context (const IOP::ServiceContext & service_context,
                                             CORBA::Boolean replace,
                                             CORBA::Environment &ACE_TRY_ENV = 
                                             TAO_default_environment ())
     ACE_THROW_SPEC ((CORBA::SystemException));    

 protected:
   CORBA::ULong request_id_;
   const char * operation_;
   Dynamic::ParameterList parameter_list_;
   Dynamic::ExceptionList exception_list_;
   Dynamic::ContextList context_list_;
   Dynamic::RequestContext request_context_;
   CORBA::Object_var forward_reference_;
   // Needed to ensure no copy anywhere. 
   IOP::ServiceContextList &service_context_list_;
   CORBA::Any_var result_;
   CORBA::Object_var target_;
   CORBA::Object_var effective_target_;
   
};

//****************************************************************               

class TAO_Export TAO_ServerRequest_Info
 : public virtual PortableInterceptor::ServerRequestInfo
{
 public:

  // This really shouldnt be this way, we need to create the lists
  // dynamically so we should generate different info objects per operation
  // and pass all args to it which will be used to creat ethe list dynamically.
  /*  ServerRequest_Info (CORBA::ULong request_id,
                      char * operation, 
                      Dynamic::ParameterList * arguments,
                      Dynamic::ExceptionList * exceptions,
                      Dynamic::ContextList * contexts,
                      Dynamic::RequestContext * operation_context,
                      CORBA::Object_ptr forward_reference,
                      PortableInterceptor::OctetSeq *object_id,
                      PortableInterceptor::OctetSeq *adapter_id,
                      CORBA::Environment &ACE_TRY_ENV = 
                      TAO_default_environment ());
  */

  // Object_id + Adapter_id nedd to be added, so shoudl this interceptor
  //  be called after the poa?
  TAO_ServerRequest_Info (const char * operation,
                      IOP::ServiceContextList &service_context_list ,               
                      CORBA::Environment &ACE_TRY_ENV = 
                      TAO_default_environment ());
    
  virtual CORBA::ULong request_id (CORBA::Environment &ACE_TRY_ENV = 
                                   TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * operation (CORBA::Environment &ACE_TRY_ENV = 
                            TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ParameterList * arguments (CORBA::Environment &ACE_TRY_ENV = 
                                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ExceptionList * exceptions (CORBA::Environment &ACE_TRY_ENV = 
                                               TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::ContextList * contexts (CORBA::Environment &ACE_TRY_ENV = 
                                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Dynamic::RequestContext * operation_context (CORBA::Environment &ACE_TRY_ENV = 
                                                       TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Any * result (CORBA::Environment &ACE_TRY_ENV = 
                               TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean response_expected (CORBA::Environment &ACE_TRY_ENV = 
                                            TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

  virtual Messaging::SyncScope sync_scope (CORBA::Environment &ACE_TRY_ENV = 
                                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual PortableInterceptor::ReplyStatus reply_status (CORBA::Environment &ACE_TRY_ENV = 
                                                         TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Object_ptr forward_reference (CORBA::Environment &ACE_TRY_ENV = 
                                               TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException)) ;

  // Probably the following methods shoudl be delegated to teh POICurrentx
  virtual CORBA::Any * get_slot (PortableInterceptor::SlotId id,
                                 CORBA::Environment &ACE_TRY_ENV = 
                                 TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::InvalidSlot));

  virtual IOP::ServiceContext * get_request_service_context (IOP::ServiceId id,
                                                             CORBA::Environment &ACE_TRY_ENV = 
                                                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IOP::ServiceContext * get_reply_service_context (IOP::ServiceId id,
                                                           CORBA::Environment &ACE_TRY_ENV = 
                                                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Any * sending_exception (CORBA::Environment &ACE_TRY_ENV = 
                                          TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  
  virtual PortableInterceptor::OctetSeq * object_id (CORBA::Environment &ACE_TRY_ENV = 
                                                     TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual PortableInterceptor::OctetSeq * adapter_id (CORBA::Environment &ACE_TRY_ENV = 
                                                      TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * target_most_derived_interface (CORBA::Environment &ACE_TRY_ENV = 
                                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr get_server_policy (CORBA::PolicyType type,
                                               CORBA::Environment &ACE_TRY_ENV = 
                                               TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_slot (PortableInterceptor::SlotId id,
                         const CORBA::Any & data,
                         CORBA::Environment &ACE_TRY_ENV = 
                         TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::InvalidSlot));

  virtual CORBA::Boolean target_is_a (const char * id,
                                      CORBA::Environment &ACE_TRY_ENV = 
                                      TAO_default_environment ())
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void add_reply_service_context (const IOP::ServiceContext & service_context,
                                          CORBA::Boolean replace,
                                          CORBA::Environment &ACE_TRY_ENV = 
                                          TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

 protected:
  CORBA::ULong request_id_;
  const char * operation_;
  Dynamic::ParameterList parameter_list_;
  Dynamic::ExceptionList  exception_list_;
  Dynamic::ContextList context_list_;
  Dynamic::RequestContext request_context_;
  CORBA::Object_var forward_reference_;
  // Needed to ensure no copy anywhere. 
  IOP::ServiceContextList &service_context_list_;
  CORBA::Any_var result_;
  PortableInterceptor::OctetSeq_var object_id_;
  PortableInterceptor::OctetSeq_var adapter_id_; 
};


//****************************************************************               

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* REQUEST_INFO_H */
