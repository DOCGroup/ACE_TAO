//
// $Id$
//

#ifndef LONGWRITES_RECEIVER_H
#define LONGWRITES_RECEIVER_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Receiver interface
/**
 * Simply print count how many bytes were received.
 */
class Receiver
  : public virtual POA_Test::Receiver
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Receiver (void);

  /// Print out the results
  void dump_results (void);

  // = The skeleton methods
  virtual void receive_data (const Test::Payload &payload,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  ACE_SYNCH_MUTEX mutex_;
  CORBA::ULong message_count_;
  CORBA::ULong byte_count_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* LONGWRITES_RECEIVER_H */
