//=============================================================================
/**
 *  @file    ImplRepo_i.h
 *
 *  $Id$
 *
 *  @author  Darrell Brunsch and Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_IMPLREPO_I_H
#define TAO_IMPLREPO_I_H

#include "ace/pre.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ImplRepoS.h"
#include "tao/ORB.h"

/**
 * @class ServerObject_i
 *
 * @brief Class for pinging a server.  Implementation Repository uses
 * this to communicate with the IMR registered servers.
 *
 * \nosubgrouping
 *
 **/
class TAO_PortableServer_Export ServerObject_i
  : public POA_ImplementationRepository::ServerObject,
    public PortableServer::RefCountServantBase
{
public:

  /// Constructor.
  ServerObject_i (CORBA::ORB_ptr orb,
                  PortableServer::POA_ptr poa);

  /// Ping method.
  void ping (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Shutdown the server.
  void shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Returns the default POA for this servant.
  PortableServer::POA_ptr _default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);

protected:
  /// Our ORB.  We need this member in case we need to shutdown the
  /// server.
  CORBA::ORB_var orb_;

  /// POA that we are registered with.
  PortableServer::POA_var poa_;
};

#include "ace/post.h"

#endif /* TAO_IMPLREPO_I_H */
