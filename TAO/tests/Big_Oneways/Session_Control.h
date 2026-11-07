
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
  explicit Session_Control (CORBA::ULong session_count);
  Session_Control (Session_Control const &) = delete;
  Session_Control (Session_Control &&) = delete;
  Session_Control &operator= (Session_Control const &) = delete;
  Session_Control &operator= (Session_Control &&) = delete;

  /// Destructor
  ~Session_Control () override;

  /// Return 1 when all sessions have finished
  bool all_sessions_finished () const;

  // = The skeleton methods
  void session_finished (CORBA::Boolean success) override;

private:
  /// Synchronize the internal state
  TAO_SYNCH_MUTEX mutex_;

  /// The type of test
  CORBA::ULong session_count_;

  /// Set to falso if any session reported a failure
  bool success_ {true};
};

#include /**/ "ace/post.h"
#endif /* BIG_ONEWAYS_SESSION_CONTROL_H */
