/**
 * @file Coordinator.h
 *
 * $Id$
 *
 */

#ifndef ECFS_COORDINATOR_H
#define ECFS_COORDINATOR_H

#include "ControlS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ECFS_Coordinator
 *
 * @brief Implement the Control::Coordinator interface
 *
 */
class ECFS_Coordinator
 : public virtual POA_Control::Coordinator
 , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  ECFS_Coordinator (int peers_expected,
                    int consumer_count,
                    int iterations,
                    int do_dump_history,
                    CORBA::ORB_ptr orb);

  /// Destructor
  virtual ~ECFS_Coordinator (void);

  //@{
  /** @name The Control::Coordinator methods
   */
  virtual void join (Control::Peer_ptr peer
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

private:
  /// Synchronize access to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// Number of peers expected
  size_t peers_expected_;

  /// Number of consumers to run on each test
  size_t consumer_count_;

  /// Number of iterations on each test
  int iterations_;

  /// This flag is set to 1 to dump the complete test history
  int do_dump_history_;

  /// Keep a reference to the ORB, used in shutdown
  CORBA::ORB_var orb_;

  /// Current number of peers
  size_t peers_count_;

  /// Peer collection
  Control::Peer_var *peers_;

};

#endif /* ECFS_COORDINATOR_H */
