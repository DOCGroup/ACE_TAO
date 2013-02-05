//
// $Id$
//

#ifndef TEST_I_H
#define TEST_I_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Sleeper interface
class Test_i : public virtual POA_Test::Sleeper
{
public:
  /// Constructor
  Test_i (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void delay (void);

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;

};

#include /**/ "ace/post.h"
#endif /* TEST_I_H */
