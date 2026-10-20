#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "HelloS.h"

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

  // = The skeleton methods
  void method (CORBA::Short count) override;

  void set_callback (Test::CallBack_ptr cb) override;


private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  Test::CallBack_var callback_;
  int count_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
