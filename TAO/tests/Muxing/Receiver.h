//
// $Id$
//

#ifndef MUXING_RECEIVER_H
#define MUXING_RECEIVER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Receiver interface
/**
 * Simply print count how many bytes were received.
 */
class Receiver
  : public POA_Test::Receiver
{
public:
  /// Constructor
  Receiver (void);

  /// Print out the results
  void dump_results (void);

  // = The skeleton methods
  virtual void receive_data (const Test::Payload &payload
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual CORBA::Long get_event_count (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  ACE_SYNCH_MUTEX mutex_;
  CORBA::ULong message_count_;
  CORBA::ULong byte_count_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* MUXING_RECEIVER_H */
