/**
 * @file Control.h
 *
 * $Id$
 *
 */

#ifndef TAO_RTEC_PERF_CONTROL_H
#define TAO_RTEC_PERF_CONTROL_H
#include "ace/pre.h"

#include "Federated_TestS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Control
 *
 * @brief Implement the Federated_Test::Control interface
 *
 */
class TAO_RTEC_Perf_Export Control
 : public virtual POA_Federated_Test::Control
 , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Control (size_t peers_expected,
           size_t iterations,
           int do_dump_history,
           CORBA::ORB_ptr orb,
           PortableServer::POA_ptr poa);

  /// Destructor
  virtual ~Control (void);

  //@{
  /** @name The Federated_Test::Control methods
   */
  virtual void join (Federated_Test::Peer_ptr peer
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

private:
  /// Synchronize access to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// Number of peers expected
  size_t peers_expected_;

  /// Number of iterations on each test
  size_t iterations_;

  /// This flag is set to 1 to dump the complete test history
  int do_dump_history_;

  /// Keep a reference to the ORB, used in shutdown
  CORBA::ORB_var orb_;

  /// The default POA
  PortableServer::POA_var poa_;

  /// Current number of items in the in the @c peers_ array.
  size_t peers_count_;

  /// An array to keep the list of peers.
  /**
   * @todo I would like to use ACE_Array<> but the interface is too
   * confusing.  std::vector<> would be better, but I'm not allowed to
   */
  Federated_Test::Peer_var *peers_;
};

#include "ace/post.h"
#endif /* TAO_RTEC_PERF_CONTROL_H */
