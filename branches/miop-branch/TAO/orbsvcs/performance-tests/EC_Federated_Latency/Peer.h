/**
 * @file Peer.h
 *
 * $Id$
 *
 */

#ifndef ECFL_PEER_H
#define ECFL_PEER_H

#include "ControlS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECFL_Peer
 *
 * @brief Implement the Control::Peer interface
 *
 */
class ECFL_Peer
 : public virtual POA_Control::Peer
 , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  ECFL_Peer (CORBA::ORB_ptr orb);

  /// Destructor
  virtual ~ECFL_Peer (void);

  /// Initialize the peer
  void init (PortableServer::POA_ptr poa
             TAO_ENV_ARG_DECL);

  //@{
  /** @name The Control::Peer methods
   */
  virtual RtecEventChannelAdmin::EventChannel_ptr
      channel (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
        ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void connect (RtecEventChannelAdmin::EventChannel_ptr remote_ec
                        TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  Control::Loopback_ptr setup_loopback (CORBA::Long experiment_id
                                        TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Control::Samples* run_experiment (CORBA::Long experiment_id,
                                            CORBA::Long iterations,
                                            CORBA::Long_out gsf
                                            TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

private:
  /// Synchronize access to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// Keep a reference to the ORB, used in shutdown
  CORBA::ORB_var orb_;

  /// Event Channel references
  RtecEventChannelAdmin::EventChannel_var event_channel_;
};

#endif /* ECFL_PEER_H */
