//
// $Id$
//

#ifndef LONGWRITES_SENDER_H
#define LONGWRITES_SENDER_H
#include "ace/pre.h"

#include "TestS.h"
#include "Sender_Task.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Sender interface
class Sender
  : public virtual POA_Test::Sender
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Sender (int test_type);

  /// Destructor
  virtual ~Sender (void);

  /// Control the type of test
  enum {
    /// Run the test using receive_data_oneway() operations
    TEST_ONEWAY,
    /// Run the test using receive_data() operations
    TEST_WRITE,
    /// Run the test using return_data() operations
    TEST_READ_WRITE
  };

  /// Run the test in a separate thread
  int run_test (CORBA::Long event_count,
                CORBA::ULong event_size);

  /// Return 1 after <shutdown> is invoked and all the messages are
  /// received.
  int test_done (CORBA::ULong message_count);

  /// Return 1 after <shutdown> is invoked
  int shutdown_called (void);

  /// Return 1 if the test is finished, assuming <message_count>
  /// messages have been received by the local Receiver
  int iteration_done (CORBA::ULong messsage_count);

  // = The skeleton methods
  virtual void add_receiver (Test::Receiver_ptr receiver,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void send_events (CORBA::Long event_count,
                            CORBA::ULong event_size,
                            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// The type of test
  int test_type_;

  /// Synchronize the internal state
  ACE_SYNCH_MUTEX mutex_;

  /// Keep track of all the receivers
  size_t receiver_count_;
  size_t receiver_length_;
  Test::Receiver_var *receivers_;

  /// Set to 1 if the shutdown() operations was called.
  int shutdown_called_;

  /// Setup event count
  CORBA::ULong event_count_;

  /// Used to run the threads
  Sender_Task sender_task_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* LONGWRITES_SENDER_H */
