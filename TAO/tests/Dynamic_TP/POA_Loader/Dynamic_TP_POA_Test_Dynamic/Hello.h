
#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::Short sleep_sec ();
  virtual void sleep_sec (CORBA::Short sleep_sec);
  virtual char * get_string (CORBA::Long);

  virtual void shutdown ();

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  CORBA::Short sleep_sec_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
