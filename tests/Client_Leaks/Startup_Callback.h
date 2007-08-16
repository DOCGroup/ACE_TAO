//
// $Id$
//

#ifndef CLIENT_LEAKS_STARTUP_CALLBACK_H
#define CLIENT_LEAKS_STARTUP_CALLBACK_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Startup_Callback interface
class Startup_Callback
  : public virtual POA_Test::Startup_Callback
{
public:
  /// Constructor
  Startup_Callback (void);

  /// Return 1 if the process has started already
  /**
   * If the process has started then <the_process> returns the object
   * reference for the new process
   */
  int process_has_started (Test::Process_out the_process);

  // = The skeleton methods
  virtual void started (Test::Process_ptr the_process);

private:
  /// Synchronize changes to the internal data
  TAO_SYNCH_MUTEX mutex_;

  /// Store the process.
  Test::Process_var process_;
};

#include /**/ "ace/post.h"
#endif /* CLIENT_LEAKS_STARTUP_CALLBACK_H */
