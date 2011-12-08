//
// $Id$
//

#ifndef SLEEP_SERVICE_H
#define SLEEP_SERVICE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Sleep_Service interface
class Sleep_Service
  : public virtual POA_Test::Sleep_Service
{
public:
  /// Constructor
  Sleep_Service (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void go_to_sleep (CORBA::ULong microseconds);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* SLEEP_SERVICE_H */
