
//=============================================================================
/**
 *  @file   Reactor_Registry.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_REACTOR_REGISTRY_H
#define TAO_REACTOR_REGISTRY_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORB_Core;
class TAO_Leader_Follower;
class TAO_Acceptor;
class ACE_Reactor;

/**
 * @class TAO_Reactor_Registry
 *
 * @brief The interface for the concurrency strategy.
 *
 * The ORB concurrency strategy is responsible for assigning
 * reactors to threads, keeping the relationship between threads
 * and their leader-follower groups and activating (if required)
 * the server-side Svc_Handlers.
 */
class TAO_Export TAO_Reactor_Registry
{
public:
  /// Default constructor
  TAO_Reactor_Registry (void);

  /// The destructor
  virtual ~TAO_Reactor_Registry (void);

  /// The ORB core for this concurrency strategy.
  TAO_ORB_Core *orb_core (void) const;

  /// Initialize the Reactor Registry
  virtual void open (TAO_ORB_Core *orb_core);

  /// Return the reactor for the current thread
  virtual ACE_Reactor *reactor (void) = 0;

  /// Return the reactor for a given acceptor
  virtual ACE_Reactor *reactor (TAO_Acceptor *acceptor) = 0;

  /// Return the Leader-Follower group for the current thread
  virtual TAO_Leader_Follower &leader_follower (void) = 0;

  /// Return the Leader-Follower group for a given acceptor
  virtual TAO_Leader_Follower &leader_follower (TAO_Acceptor *acceptor) = 0;

  /**
   * The strategy is allowed to store TSS resources using a
   * place-holder in the ORB_Core class. The ORB_Core the calls back
   * to do the final cleanup.
   */
  virtual void destroy_tss_cookie (void* cookie) = 0;

  /// Wakeup all the reactors
  virtual int shutdown_all (void) = 0;

private:
  /// The orb_core
  TAO_ORB_Core *orb_core_;
};

#if defined (__ACE_INLINE__)
# include "tao/Reactor_Registry.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_REACTOR_REGISTRY_H */
