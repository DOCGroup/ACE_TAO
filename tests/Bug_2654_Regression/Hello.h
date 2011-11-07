// $Id$

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
  Hello (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void method (CORBA::Short count);

  virtual void set_callback (Test::CallBack_ptr cb);


private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
  Test::CallBack_var callback_;
  int count_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
