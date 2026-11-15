
#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  Hello (CORBA::ORB_ptr orb);

  Test::Octet_Seq *get_big_reply (CORBA::ULong size) override;
  void big_request (::Test::Octet_Seq const &octet_in) override;

  /// @name Skeleton methods
  /// @{
  char *get_string (char const *mystring) override;

  void shutdown () override;
  /// @}

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
