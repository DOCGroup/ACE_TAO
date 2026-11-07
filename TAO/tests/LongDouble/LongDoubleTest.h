
#ifndef LONGDOUBLETEST_H
#define LONGDOUBLETEST_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::LongDoubleTest interface
class LongDoubleTest
  : public virtual POA_Test::LongDoubleTest
{
public:
  /// Constructor
  explicit LongDoubleTest (CORBA::ORB_ptr orb);
  LongDoubleTest (LongDoubleTest const &) = delete;
  LongDoubleTest (LongDoubleTest &&) = delete;
  LongDoubleTest &operator= (LongDoubleTest const &) = delete;
  LongDoubleTest &operator= (LongDoubleTest &&) = delete;

  // = The skeleton methods
  CORBA::LongDouble get_long_double () override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* LONGDOUBLETEST_H */
