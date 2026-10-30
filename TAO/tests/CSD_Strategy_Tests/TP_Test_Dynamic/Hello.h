
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

  // = The skeleton methods
  char * get_string () override;

  void shutdown () override;

  Hello (Hello const &) = delete;
  Hello (Hello &&) = delete;
  Hello & operator= (Hello const &) = delete;
  Hello & operator= (Hello &&) = delete;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
