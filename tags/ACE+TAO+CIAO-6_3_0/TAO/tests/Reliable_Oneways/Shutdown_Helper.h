//
// $Id$
//

#ifndef SHUTDOWN_HELPER_H
#define SHUTDOWN_HELPER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Shutdown_Helper interface
class Shutdown_Helper
  : public virtual POA_Test::Shutdown_Helper
{
public:
  /// Constructor
  Shutdown_Helper (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* SHUTDOWN_HELPER_H */
