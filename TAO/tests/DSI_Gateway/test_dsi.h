
//=============================================================================
/**
 *  @file   test_i.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_DSI_GATEWAY_TEST_DSI_H
#define TAO_DSI_GATEWAY_TEST_DSI_H

#include "tao/DynamicInterface/Server_Request.h"
#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/ORB.h"

/**
 * @class DSI_Simple_Server
 *
 * @brief DSI Simpler Server implementation
 *
 * Implements the DSI/DII gateway.
 */
class DSI_Simple_Server : public TAO_DynamicImplementation
{
public:
  /// ctor
  DSI_Simple_Server (CORBA::ORB_ptr orb,
                     CORBA::Object_ptr target,
                     PortableServer::POA_ptr poa);

  // = The DynamicImplementation methods.
  virtual void invoke (CORBA::ServerRequest_ptr request);

  virtual CORBA::RepositoryId _primary_interface (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa
    );

  virtual PortableServer::POA_ptr _default_POA (
    );

private:
  /// The ORB
  CORBA::ORB_var orb_;

  /// Target object, forward requests to it...
  CORBA::Object_var target_;

  /// The POA
  PortableServer::POA_var poa_;
};

#if defined(__ACE_INLINE__)
#include "test_dsi.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_DSI_GATEWAY_TEST_I_H */
