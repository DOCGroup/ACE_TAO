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

#include "tao/PortableServer/PortableServerC.h"

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
class ImR_Forwarder: public PortableServer::ServantLocator
{
public:
  ImR_Forwarder (ImR_Locator_i *ir_impl, CORBA::ORB_ptr orb);

  /// Called before the invocation begins.
  virtual PortableServer::Servant preinvoke (
    const PortableServer::ObjectId &oid,
    PortableServer::POA_ptr poa,
    const char * operation,
    PortableServer::ServantLocator::Cookie &cookie
    ACE_ENV_ARG_DECL
  ) ACE_THROW_SPEC ((CORBA::SystemException, PortableServer::ForwardRequest));

  /// Called after the invocation finishes.
  virtual void postinvoke (const PortableServer::ObjectId &oid,
                           PortableServer::POA_ptr poa,
                           const char * operation,
                           PortableServer::ServantLocator::Cookie cookie,
                           PortableServer::Servant servant
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Where we find out where to forward to.
  ImR_Locator_i *imr_impl_;

  /// POA reference.
  PortableServer::Current_var poa_current_var_;

  /// Variable to save the ORB reference passed to the constr.
  CORBA::ORB_ptr orb_;
};

#endif /* IMR_FORWARDER_H */
