// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerObject_i.h
 *
 *  $Id$
 *
 *  @author  Darrell Brunsch
 *  @author  Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_SERVEROBJECT_I_H
#define TAO_SERVEROBJECT_I_H

#include /**/ "ace/pre.h"

#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ImR_Client/ServerObjectS.h"
#include "tao/ImR_Client/imr_client_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ServerObject_i
 *
 * @brief Class for pinging a server.  Implementation Repository uses
 * this to communicate with the IMR registered servers.
 *
 * \nosubgrouping
 *
 **/
class TAO_IMR_Client_Export ServerObject_i
  : public POA_ImplementationRepository::ServerObject
{
public:
  /// Constructor.
  ServerObject_i (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa);

  /// Ping method.
  void ping (void);

  /// Shutdown the server.
  void shutdown (void);

  /// Returns the default POA for this servant.
  PortableServer::POA_ptr _default_POA (void);

protected:
  /// Our ORB.  We need this member in case we need to shutdown the
  /// server.
  CORBA::ORB_var orb_;

  /// POA that we are registered with.
  PortableServer::POA_var poa_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_SERVEROBJECT_I_H */
