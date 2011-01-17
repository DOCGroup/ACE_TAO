// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/DSI_Gateway
//
// = FILENAME
//   test_i.h
//
// = AUTHOR
//   Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_DSI_GATEWAY_TEST_DSI_H
#define TAO_DSI_GATEWAY_TEST_DSI_H

#include "tao/DynamicInterface/Server_Request.h"
#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/ORB.h"

class DSI_Simple_Server : public TAO_DynamicImplementation
{
  // = TITLE
  //   DSI Simpler Server implementation
  //
  // = DESCRIPTION
  //   Implements the DSI/DII gateway.
  //
public:
  DSI_Simple_Server (CORBA::ORB_ptr orb,
                     CORBA::Object_ptr target,
                     PortableServer::POA_ptr poa);
  // ctor

  // = The DynamicImplementation methods.
  virtual void invoke (CORBA::ServerRequest_ptr request);

  virtual CORBA::RepositoryId _primary_interface (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa
    );

  virtual PortableServer::POA_ptr _default_POA (
    );

private:
  CORBA::ORB_var orb_;
  // The ORB

  CORBA::Object_var target_;
  // Target object, forward requests to it...

  PortableServer::POA_var poa_;
  // The POA
};

#if defined(__ACE_INLINE__)
#include "test_dsi.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_DSI_GATEWAY_TEST_I_H */
