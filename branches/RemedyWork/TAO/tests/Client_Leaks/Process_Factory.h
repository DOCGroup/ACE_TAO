//
// $Id$
//

#ifndef CLIENT_LEAKS_PROCESS_FACTORY_H
#define CLIENT_LEAKS_PROCESS_FACTORY_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Process_Factory interface
class Process_Factory
  : public virtual POA_Test::Process_Factory
{
public:
  /// Constructor
  Process_Factory (CORBA::ORB_ptr orb);

  /// Return 1 if the shutdown message has been received already
  int shutdown_received (void);

  // = The skeleton methods
  virtual Test::Process_ptr create_new_process (void);

  virtual void noop (void);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;

  /// Set to 1 when the test has finished
  int shutdown_received_;
};

#include /**/ "ace/post.h"
#endif /* CLIENT_LEAKS_PROCESS_FACTORY_H */
