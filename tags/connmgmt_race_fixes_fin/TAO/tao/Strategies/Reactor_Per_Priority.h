// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Reactor_Per_Priority.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_REACTOR_PER_PRIORITY_H
#define TAO_REACTOR_PER_PRIORITY_H
#include "ace/pre.h"

#include "tao/Reactor_Registry.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "strategies_export.h"
#include "ace/Map_Manager.h"

class TAO_Leader_Follower;

class TAO_Strategies_Export TAO_Reactor_Per_Priority  : public TAO_Reactor_Registry
{
  // = TITLE
  //   The Reactor_Per_Priority concurrency strategy.
  //
  // = DESCRIPTION
  //   This strategy creates a different reactor for each priority
  //   level, threads at the right priority level run the event loop
  //   on that reactor.
  //   Multiple threads can share the same reactor, usually using the
  //   thread-pool strategy.
  //
public:
  TAO_Reactor_Per_Priority (void);
  // Default constructor

  virtual ~TAO_Reactor_Per_Priority (void);
  // The destructor

  // = The TAO_Reactor_Registry methods, please check the
  // documentation in tao/Reactor_Registry.h
  virtual ACE_Reactor *reactor (void);
  virtual ACE_Reactor *reactor (TAO_Acceptor *acceptor);
  virtual TAO_Leader_Follower &leader_follower (void);
  virtual TAO_Leader_Follower &leader_follower (TAO_Acceptor *acceptor);
  virtual void destroy_tss_cookie (void* cookie);
  virtual int shutdown_all (void);

private:
  TAO_Leader_Follower *leader_follower_i (CORBA::Short priority);
  // Obtain the leader follower object given a priority, used to
  // implement all the other methods

private:
  typedef ACE_Map_Manager<CORBA::Short,TAO_Leader_Follower*,TAO_SYNCH_MUTEX> Map;
  typedef ACE_Map_Iterator<CORBA::Short,TAO_Leader_Follower*,TAO_SYNCH_MUTEX> Map_Iterator;

  Map map_;
  // The map between priorities and the control structure for the
  // reactor
};

#if defined (__ACE_INLINE__)
# include "Reactor_Per_Priority.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_REACTOR_PER_PRIORITY_H */
