//
// $Id$
//

#ifndef BIG_ONEWAYS_SESSION_H
#define BIG_ONEWAYS_SESSION_H
#include "ace/pre.h"

#include "TestS.h"
#include "Session_Task.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Session interface
class Session
  : public virtual POA_Test::Session
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Session (Test::Session_Control_ptr control);

  /// Destructor
  virtual ~Session (void);

  /// Run one of the experiment threads
  int svc (void);

  // = The skeleton methods
  virtual void start (const Test::Session_List &other_sessions,
                      CORBA::ULong payload_size,
                      CORBA::ULong thread_count,
                      CORBA::ULong message_count,
                      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Test::Already_Running,
                     Test::No_Peers));

  virtual void receive_payload (const Test::Payload &the_payload,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Helper function used to report any problems and destroy local
  /// resources
  void terminate (CORBA::Boolean success,
                  CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC (());

  /// Helper function used to report any problems and destroy local
  /// resources
  void check_for_termination (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC (());

private:
  /// Synchronize the internal state
  ACE_SYNCH_MUTEX mutex_;

  /// Keep a reference to the Session_Control, this is used to report
  /// when the test finishes.
  Test::Session_Control_var control_;

  /// Keep track of wether the test is running.
  int running_;

  /// The other session objects participating in the test
  Test::Session_List other_sessions_;

  /// Size of each message
  CORBA::ULong payload_size_;

  /// Number of threads
  CORBA::ULong thread_count_;

  /// Number of messages to send
  CORBA::ULong message_count_;

  /// The number of threads currently running, when this reaches 0 the
  /// session destroys itself.
  int active_thread_count_;

  /// Number of messages expected
  CORBA::ULong expected_messages_;

  /// Helper class to run svc() in a separate thread
  Session_Task task_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* BIG_ONEWAYS_SESSION_H */
