// -*- C++ -*-

//=============================================================================
/**
 *  @file   Request_Context_Repository.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef REQUEST_CONTEXT_REPOSITORY_H
#define REQUEST_CONTEXT_REPOSITORY_H

#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "tao/PortableInterceptorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FTEC_Event_Channel_Impl;

namespace PortableInterceptor
{
  class ORBInitInfo;
  typedef ORBInitInfo *ORBInitInfo_ptr;

  class ServerRequestInfo;
  typedef ServerRequestInfo *ServerRequestInfo_ptr;

  class ClientRequestInfo;
  typedef ClientRequestInfo *ClientRequestInfo_ptr;
}

/**
 * @class Request_Context_Repository
 *
 * @brief This class is used to transfer the information between thread context
 *        information and the request context in the portable interceptors. It
 *        encapulates the PICurrent to provide a type safe way for the transfer of
 *        FTEC specific information inside and outside protable interceptors.
 */
class Request_Context_Repository
{
public:

  void init(CORBA::ORB_ptr orb);

  /**
   * Used by ORBInitializer to allocate required slots for
   * FTEC.
   */
  void allocate_slots(PortableInterceptor::ORBInitInfo_ptr info
            ACE_ENV_ARG_DECL);


  void generate_object_id(FtRtecEventChannelAdmin::ObjectId& object_id
                          ACE_ENV_ARG_DECL);
  void set_object_id(const FtRtecEventChannelAdmin::ObjectId& object_id
                     ACE_ENV_ARG_DECL);
  FtRtecEventChannelAdmin::ObjectId_var get_object_id(ACE_ENV_SINGLE_ARG_DECL);
  FtRtecEventChannelAdmin::ObjectId_var
    get_object_id(PortableInterceptor::ServerRequestInfo_ptr ri
                  ACE_ENV_ARG_DECL);

  void set_cached_result(PortableInterceptor::ServerRequestInfo_ptr ri,
                         const CORBA::Any& result
                         ACE_ENV_ARG_DECL);

  CORBA::Any_ptr get_cached_result(ACE_ENV_SINGLE_ARG_DECL);
  bool is_executed_request();

  void set_sequence_number(PortableInterceptor::ServerRequestInfo_ptr ri,
                           FTRT::SequenceNumber
                           ACE_ENV_ARG_DECL);

  void set_sequence_number(FTRT::SequenceNumber
                           ACE_ENV_ARG_DECL);
  FTRT::SequenceNumber get_sequence_number(PortableInterceptor::ClientRequestInfo_ptr ri
                           ACE_ENV_ARG_DECL);

  FTRT::SequenceNumber get_sequence_number(ACE_ENV_SINGLE_ARG_DECL);

  void set_ft_request_service_context(
    PortableInterceptor::ServerRequestInfo_ptr ri,
    IOP::ServiceContext_var service_context
    ACE_ENV_ARG_DECL);

  CORBA::Any_var get_ft_request_service_context(
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL);

  void set_transaction_depth(
    PortableInterceptor::ServerRequestInfo_ptr ri,
    FTRT::TransactionDepth depth
    ACE_ENV_ARG_DECL);

 void set_transaction_depth(
    FTRT::TransactionDepth depth
    ACE_ENV_ARG_DECL);

 FTRT::TransactionDepth get_transaction_depth(
    ACE_ENV_SINGLE_ARG_DECL);

 FTRT::TransactionDepth get_transaction_depth(
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /// REQUEST_CONTEXT_REPOSITORY_H
