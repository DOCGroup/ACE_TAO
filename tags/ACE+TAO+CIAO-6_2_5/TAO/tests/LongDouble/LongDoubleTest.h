//
// $Id$
//

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
  LongDoubleTest (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::LongDouble get_long_double ();

  virtual void shutdown ();

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* LONGDOUBLETEST_H */
