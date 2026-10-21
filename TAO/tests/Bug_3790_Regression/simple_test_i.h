
#ifndef HELLO_H
#define HELLO_H

#include "simple_testS.h"

class Simple_Test_i
  : public virtual POA_simple::SimpleTest_T
{
public:
  /// Constructor
  Simple_Test_i (CORBA::ORB_ptr orb);
  Simple_Test_i (Simple_Test_i const &) = delete;
  Simple_Test_i (Simple_Test_i &&) = delete;
  Simple_Test_i &operator= (Simple_Test_i const &) = delete;
  Simple_Test_i &operator= (Simple_Test_i &&) = delete;

  // = The skeleton methods
  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#endif /* HELLO_H */
