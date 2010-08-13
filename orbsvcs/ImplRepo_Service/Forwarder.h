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

#endif /* IMR_FORWARDER_H */
