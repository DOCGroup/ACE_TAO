//
// $Id$
//

#ifndef STACK_RECURSION_SENDER_H
#define STACK_RECURSION_SENDER_H
#include /**/ "ace/pre.h"

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
class Sender
  : public virtual POA_Test::Sender
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Sender (CORBA::ORB_ptr orb);

  /// Print out the results
  void dump_results (void);

  /// Get the status of the flag..
  int is_done (void) const;

  // = The skeleton methods
  virtual CORBA::Boolean get_data (CORBA::ULong size,
                                   Test::Payload_out payload
                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Long get_event_count (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void ping (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  ACE_SYNCH_MUTEX mutex_;
  CORBA::ULong message_count_;
  CORBA::ULong byte_count_;
  CORBA::ORB_var orb_;

  int is_done_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* MUXING_RECEIVER_H */
