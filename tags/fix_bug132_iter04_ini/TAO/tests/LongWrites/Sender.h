//
// $Id$
//

#ifndef LONGWRITES_SENDER_H
#define LONGWRITES_SENDER_H
#include "ace/pre.h"

#include "TestS.h"

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
  Sender (void);

  /// Destructor
  virtual ~Sender (void);

  /// Return 1 after <shutdown> is invoked
  int shutdown_called (void);

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
  ACE_SYNCH_MUTEX mutex_;

  size_t receiver_count_;
  size_t receiver_length_;
  Test::Receiver_var *receivers_;

  int shutdown_called_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* LONGWRITES_SENDER_H */
