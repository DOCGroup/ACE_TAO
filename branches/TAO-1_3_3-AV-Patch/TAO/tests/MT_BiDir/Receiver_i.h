//
// $Id$
//

#ifndef MT_BIDIR_RECEIVER_I_H
#define MT_BIDIR_RECEIVER_I_H
#include /**/ "ace/pre.h"

#include "ReceiverS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/**
 * Simply print count how many bytes were received.
 */
class Receiver_i
  : public virtual POA_Receiver
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Receiver_i (void);


  /// Get the number of events that have been received, local method
  CORBA::Long get_event_count (void);

  // = The skeleton methods
  virtual void receive_payload (const Receiver::Payload &payload
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  TAO_SYNCH_MUTEX mutex_;
  CORBA::ULong message_count_;
  CORBA::ULong byte_count_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* MUXING_RECEIVER_H */
