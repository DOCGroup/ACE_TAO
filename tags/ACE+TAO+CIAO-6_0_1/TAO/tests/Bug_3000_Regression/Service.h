//
// $Id$
//

#ifndef CALLBACK_SERVICE_H
#define CALLBACK_SERVICE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Service interface
class Service
  : public virtual POA_Test::Service
{
public:
  /// Constructor
  Service (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void run_test (Test::Callback_ptr callback);

  virtual void shutdown (void);

private:

  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* CALLBACK_SERVICE_H */
