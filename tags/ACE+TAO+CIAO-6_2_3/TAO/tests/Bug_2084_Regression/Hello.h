// -*- C++ -*-
//
// $Id$
//

#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#include "ace/OS_NS_Thread.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb,
         ACE_thread_t thr_id);

  // = The skeleton methods
  virtual char * get_string ( ::Test::ThreadId caller_threadid);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  ACE_thread_t const thr_id_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
