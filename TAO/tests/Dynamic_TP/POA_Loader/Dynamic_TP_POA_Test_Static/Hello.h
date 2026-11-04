
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
  explicit Hello (CORBA::ORB_ptr orb);
  Hello (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello &operator= (Hello const &) = delete;
  Hello &operator= (Hello &&) = delete;

  // = The skeleton methods
  CORBA::Short sleep_sec () override;
  void sleep_sec (CORBA::Short sleep_sec) override;
  char * get_string (CORBA::Long) override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_ {};
  CORBA::Short sleep_sec_ {1};
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
