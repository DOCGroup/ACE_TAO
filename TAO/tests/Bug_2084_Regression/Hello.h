// -*- C++ -*-

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
  explicit Hello (CORBA::ORB_ptr orb);
  Hello (Hello const &) = delete;
  Hello &operator= (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello &operator= (Hello &&) = delete;

  /// Returns a CORBA-allocated string; the generated mapping transfers it to the caller.
  char *get_string (::Test::ThreadId caller_threadid) override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var const orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
