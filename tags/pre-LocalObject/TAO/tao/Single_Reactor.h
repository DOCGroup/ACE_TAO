// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Single_Reactor.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_SINGLE_REACTOR_H
#define TAO_SINGLE_REACTOR_H
#include "ace/pre.h"

#include "tao/Reactor_Registry.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Leader_Follower;

class TAO_Export TAO_Single_Reactor  : public TAO_Reactor_Registry
{
  // = TITLE
  //   The Single_Reactor concurrency strategy.
  //
  // = DESCRIPTION
  //   This strategy creates a different reactor for each priority
  //   level, threads at the right priority level run the event loop
  //   on that reactor.
  //   Multiple threads can share the same reactor, usually using the
  //   thread-pool strategy.
  //
public:
  TAO_Single_Reactor (void);
  // Default constructor

  virtual ~TAO_Single_Reactor (void);
  // The destructor

  // = The TAO_Reactor_Registry methods, please check the
  // documentation in tao/Reactor_Registry.h
  virtual void open (TAO_ORB_Core* orb_core);
  virtual ACE_Reactor *reactor (void);
  virtual ACE_Reactor *reactor (TAO_Acceptor *acceptor);
  virtual TAO_Leader_Follower &leader_follower (void);
  virtual TAO_Leader_Follower &leader_follower (TAO_Acceptor *acceptor);
  virtual void destroy_tss_cookie (void* cookie);
  virtual int shutdown_all (void);

private:
  TAO_Leader_Follower *leader_follower_;
  // The leader follower
};

#if defined (__ACE_INLINE__)
# include "tao/Single_Reactor.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_SINGLE_REACTOR_H */
