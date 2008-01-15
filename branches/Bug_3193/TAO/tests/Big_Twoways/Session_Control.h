//
// $Id$
//

#ifndef BIG_ONEWAYS_SESSION_CONTROL_H
#define BIG_ONEWAYS_SESSION_CONTROL_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Session_Control interface
class Session_Control
  : public virtual POA_Test::Session_Control
{
public:
  /// Constructor
  /**
   * @param session_count Number of session objects in the experiment.
   */
  Session_Control (CORBA::ULong session_count);

  /// Destructor
  virtual ~Session_Control (void);

  /// Return 1 when all sessions have finished
  int all_sessions_finished (void) const;

  // = The skeleton methods
  virtual void session_finished (CORBA::Boolean success);

private:
  /// Synchronize the internal state
  TAO_SYNCH_MUTEX mutex_;

  /// The type of test
  CORBA::ULong session_count_;

  /// Set to falso if any session reported a failure
  CORBA::Boolean success_;
};

#include /**/ "ace/post.h"
#endif /* BIG_ONEWAYS_SESSION_CONTROL_H */
