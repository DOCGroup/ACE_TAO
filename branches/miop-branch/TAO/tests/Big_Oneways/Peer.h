//
// $Id$
//

#ifndef BIG_ONEWAYS_PEER_H
#define BIG_ONEWAYS_PEER_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Peer interface
class Peer
  : public virtual POA_Test::Peer
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Peer (CORBA::ORB_ptr orb);

  /// Destructor
  virtual ~Peer (void);

  // = The skeleton methods
  virtual Test::Session_ptr create_session (Test::Session_Control_ptr control,
                                            CORBA::ULong payload_size,
                                            CORBA::ULong thread_count,
                                            CORBA::ULong message_count,
                                            CORBA::ULong peer_count
                                            TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Keep a pointer to the ORB to shutdown cleanly
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* BIG_ONEWAYS_PEER_H */
