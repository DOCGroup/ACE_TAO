//
// $Id$
//

#ifndef THROUGHPUT_RECEIVER_H
#define THROUGHPUT_RECEIVER_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Receiver interface
class Receiver
  : public virtual POA_Test::Receiver
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Receiver (void);

  // = The skeleton methods
  virtual void receive_data (const Test::Message &message
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void done (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// The timestamp for the first message
  ACE_hrtime_t start_time_;

  /// The number of messages received
  size_t message_count_;

  /// The number of bytes received
  size_t byte_count_;

  /// The timestamp for the last message
  ACE_hrtime_t last_message_time_;

  /// The id for the last message
  CORBA::ULong last_message_id_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* THROUGHPUT_RECEIVER_H */
