// -*- C++ -*-
//
// $Id$
//

#ifndef EVENT_NODE_H
#define EVENT_NODE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#include "ace/OS_NS_Thread.h"

/// Implement the Test::EventNode interface
class EventNode
  : public virtual POA_Test::EventNode
{
public:
  /// Constructor
  EventNode (CORBA::ORB_ptr orb,
             ACE_thread_t thr_id);

  // = The skeleton methods
  virtual void registerHello ( ::Test::Hello_ptr h);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  ACE_thread_t const thr_id_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
