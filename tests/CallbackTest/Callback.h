//
// $Id$
//

#ifndef CALLBACK_CALLBACK_H
#define CALLBACK_CALLBACK_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Callback interface
class Callback
  : public virtual POA_Test::Callback
{
public:
  /// Constructor
  Callback (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual CORBA::Boolean are_you_there (CORBA::String_out answer);

  virtual void test_oneway (void);

  virtual void shutdown (void);
private:
  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* CALLBACK_CALLBACK_H */
