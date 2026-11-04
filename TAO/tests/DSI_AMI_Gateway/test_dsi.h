
//=============================================================================
/**
 *  @file   test_dsi.h
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_DSI_GATEWAY_TEST_DSI_H
#define TAO_DSI_GATEWAY_TEST_DSI_H

#include "tao/DynamicInterface/Server_Request.h"
#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/DynamicInterface/AMH_DSI_Response_Handler.h"
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
  DSI_Simple_Server (DSI_Simple_Server const &) = delete;
  DSI_Simple_Server (DSI_Simple_Server &&) = delete;
  DSI_Simple_Server &operator= (DSI_Simple_Server const &) = delete;
  DSI_Simple_Server &operator= (DSI_Simple_Server &&) = delete;

  /**
   * Turns around and calls invoke, but using AMH style handler
   * = The DynamicImplementation methods.
   *
   */
  void _dispatch (TAO_ServerRequest &request,
                  TAO::Portable_Server::Servant_Upcall *context) override;

  // we need this because it is a pure virtual method in
  // its parent class
  void invoke (CORBA::ServerRequest_ptr request) override;
  void invoke (CORBA::ServerRequest_ptr request,
               TAO_AMH_DSI_Response_Handler_ptr rh);

  CORBA::RepositoryId _primary_interface (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa) override;

  PortableServer::POA_ptr _default_POA () override;

private:
  /// The ORB
  CORBA::ORB_var const orb_;

  /// Target object, forward requests to it...
  CORBA::Object_var const target_;

  /// The POA
  PortableServer::POA_var const poa_;
};

#if defined(__ACE_INLINE__)
#include "test_dsi.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_DSI_GATEWAY_TEST_I_H */
