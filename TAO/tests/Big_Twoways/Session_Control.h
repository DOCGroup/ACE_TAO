//
// $Id$
//

#ifndef BIG_ONEWAYS_SESSION_CONTROL_H
#define BIG_ONEWAYS_SESSION_CONTROL_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Session_Control interface
class Session_Control
  : public virtual POA_Test::Session_Control
  , public virtual PortableServer::RefCountServantBase
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
  virtual void session_finished (CORBA::Boolean success
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Synchronize the internal state
  ACE_SYNCH_MUTEX mutex_;

  /// The type of test
  CORBA::ULong session_count_;

  /// Set to falso if any session reported a failure
  CORBA::Boolean success_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* BIG_ONEWAYS_SESSION_CONTROL_H */
