// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_DynamicImplementation.h
 *
 *  $Id$
 *
 *  @author Jon Astle (jon@astle45.fsnet.co.uk)
 */
//=============================================================================


#ifndef TAO_CEC_DYNAMICIMPLEMENTATION_H
#define TAO_CEC_DYNAMICIMPLEMENTATION_H

#include "orbsvcs/CosEvent/CEC_TypedProxyPushConsumer.h"
#include "orbsvcs/CosEvent/CEC_TypedEventChannel.h"
#include "tao/DynamicInterface/Server_Request.h"
#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/ORB.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * DSI TAO_CEC_DynamicImplementationServer implementation
 *
 * Implements the DSI impl object
 */
class TAO_CEC_DynamicImplementationServer : public TAO_DynamicImplementation
{
public:

  /// Constructor
  TAO_CEC_DynamicImplementationServer (PortableServer::POA_ptr poa,
                                       TAO_CEC_TypedProxyPushConsumer *typed_pp_consumer,
                                       TAO_CEC_TypedEventChannel *typed_event_channel);

  /// Destructor
  virtual ~TAO_CEC_DynamicImplementationServer (void);

  // = The DynamicImplementation methods.
  virtual void invoke (CORBA::ServerRequest_ptr request);

  virtual CORBA::RepositoryId _primary_interface (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa);

  virtual PortableServer::POA_ptr _default_POA (void);

  /// Handles the _is_a call
  virtual void is_a (CORBA::ServerRequest_ptr request);

private:
  /// The POA
  PortableServer::POA_var poa_;

  /// The Typed Proxy Push Consumer Implementation
  TAO_CEC_TypedProxyPushConsumer *typed_pp_consumer_;

  /// The Typed Event Channel Implementation
  TAO_CEC_TypedEventChannel *typed_event_channel_;

  /// The RepositoryId
  CORBA::RepositoryId repository_id_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_DynamicImplementation.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CEC_DYNAMICIMPLEMENTATION_H */
