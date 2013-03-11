// -*- C++ -*-
//=============================================================================
/**
 *  @file   Forwarder.h
 *
 *  $Id$
 *
 *  @brief  This class implements ImR's forwarding ServantLocator
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 *  @author Priyanka Gontla <pgontla@doc.ece.uci.edu>
 */
//=============================================================================

#ifndef IMR_FORWARDER_H
#define IMR_FORWARDER_H

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/DynamicInterface/AMH_DSI_Response_Handler.h"
#include "tao/Messaging/Messaging.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"


class ImR_Locator_i;

/**
 * @class ImR_Forwarder:
 *
 * @brief Implementation Repository Forwarder
 *
 * This class provides a ServantLocator implementation that
 * is used to handle arbitrary calls and forward them to the
 * correct place.
 */
class ImR_Forwarder
  : public PortableServer::ServantLocator,
    public CORBA::LocalObject
{
public:
  ImR_Forwarder (ImR_Locator_i& imr_impl);

  /// Called before the invocation begins.
  virtual PortableServer::Servant preinvoke (
    const PortableServer::ObjectId &oid,
    PortableServer::POA_ptr poa,
    const char * operation,
    PortableServer::ServantLocator::Cookie &cookie);

  virtual void postinvoke (
    const PortableServer::ObjectId & oid,
    PortableServer::POA_ptr adapter,
    const char * operation,
    PortableServer::ServantLocator::Cookie the_cookie,
    PortableServer::Servant the_servant);

  void init(CORBA::ORB_ptr orb);

private:
  /// Where we find out where to forward to.
  ImR_Locator_i& locator_;

  /// POA reference.
  PortableServer::Current_var poa_current_var_;

  /// Variable to save the ORB reference passed to the constr.
  CORBA::ORB_ptr orb_;
};

// forward declare the types used to manage AMH/DSI coupling
class TAO_AMH_DSI_Response_Handler;
typedef TAO_AMH_DSI_Response_Handler * TAO_AMH_DSI_Response_Handler_ptr;
class TAO_AMH_DSI_Response_Handler_var;

class ImR_ReplyHandler
{
public:
  ImR_ReplyHandler (const char *key,
                    const char *server_name,
                    CORBA::ORB_ptr orb,
                    TAO_AMH_DSI_Response_Handler_ptr resp);

  ~ImR_ReplyHandler (void);

  void send_ior (const char *pior);
  void send_exception (void);

private:
  CORBA::String_var key_str_;
  CORBA::String_var server_name_;
  CORBA::ORB_var orb_;
  TAO_AMH_DSI_Response_Handler_var resp_;
};

class ImR_DSI_Forwarder : public virtual TAO_DynamicImplementation
{
public:
  ImR_DSI_Forwarder (ImR_Locator_i& imr_impl);
  virtual ~ImR_DSI_Forwarder (void);

  virtual void _dispatch (TAO_ServerRequest& request,
                          TAO::Portable_Server::Servant_Upcall *context);

  void invoke (CORBA::ServerRequest_ptr request);

  char * _primary_interface(const PortableServer::ObjectId&,
                            PortableServer::POA_ptr);

  void invoke (CORBA::ServerRequest_ptr request,
               TAO_AMH_DSI_Response_Handler_ptr rh);

  void invoke_get_interface(CORBA::ServerRequest_ptr request);

  void invoke_primary_interface(CORBA::ServerRequest_ptr request);


  void init(CORBA::ORB_ptr orb);

private:
  /// Where we find out where to forward to.
  ImR_Locator_i& locator_;

  /// POA reference.
  PortableServer::Current_var poa_current_var_;

  /// Variable to save the ORB reference passed to the constr.
  CORBA::ORB_ptr orb_;
};


#endif /* IMR_FORWARDER_H */
