//
// $Id$
//

#ifndef MT_BIDIR_RECEIVER_H
#define MT_BIDIR_RECEIVER_H
#include /**/ "ace/pre.h"

#include "SenderS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class ACE_Manual_Event;

/**
 * Simply print count how many bytes were received.
 */
class Sender_i
  : public virtual POA_Sender
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Sender_i (int no_clients,
            ACE_Manual_Event &event);

  // = The skeleton methods
  virtual CORBA::Long receiver_object (Receiver *recv
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Sender::Table_Full));

  /// Public method defined locally
  void send_message (ACE_ENV_SINGLE_ARG_DECL);

private:
  /// Synchronizing acces to this class
  TAO_SYNCH_MUTEX mutex_;

  /// An event for waking other threads
  ACE_Manual_Event &event_;

  /// An array of receiver pointers
  Receiver **receivers_;

  /// Size of the <this->receivers_> array
  int no_clients_;

  /// Receiver index
  int last_index_;

  /// Payload that is being sent
  Receiver::Payload payload_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* MUXING_RECEIVER_H */
