/**
 * @file Peer_Base.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 */

#ifndef TAO_RTEC_PERF_PEER_BASE_H
#define TAO_RTEC_PERF_PEER_BASE_H

#include "Federated_TestS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class RTServer_Setup;

/**
 * @class Peer_Base
 *
 * @brief Implement the shared portions of Federated_Test::Peer
 *        servants
 *
 */
class Peer_Base
 : public virtual POA_Federated_Test::Peer
 , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Peer_Base (CORBA::ORB_ptr orb,
             RTServer_Setup &rtserver_setup
             ACE_ENV_ARG_DECL);

  /// Destructor
  virtual ~Peer_Base (void);

  //@{
  /** @name The Federated_Test::Peer methods
   */
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Object_ptr
      channel (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void connect (Federated_Test::Peer_ptr remote_peer
                        ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  Federated_Test::Loopback_ptr setup_loopback (CORBA::Long experiment_id,
                                               CORBA::Long base_event_type
                                               ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

protected:
  /// Keep a reference to the ORB, used in shutdown
  CORBA::ORB_var orb_;

  /// The POA used for the suppliers, consumers, activate the EC and
  /// as this servant's _default_POA()
  PortableServer::POA_var poa_;

  /// Event Channel references
  RtecEventChannelAdmin::EventChannel_var event_channel_;
};

#endif /* TAO_RTEC_PERF_PEER_BASE_H */
