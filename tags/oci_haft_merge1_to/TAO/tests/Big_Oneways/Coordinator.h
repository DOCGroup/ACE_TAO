//
// $Id$
//

#ifndef BIG_ONEWAYS_COORDINATOR_H
#define BIG_ONEWAYS_COORDINATOR_H
#include "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Coordinator interface
class Coordinator
  : public virtual POA_Test::Coordinator
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Coordinator (CORBA::ULong peer_count);


  /// Check if all the peers have registered already
  int has_all_peers (void) const;

  /// Check a session on each peer
  void create_session_list (Test::Session_Control_ptr session_control,
                            CORBA::ULong payload_size,
                            CORBA::ULong thread_count,
                            CORBA::ULong message_count,
                            Test::Session_List &session_list
                            ACE_ENV_ARG_DECL);

  /// Shutdown all the peers
  void shutdown_all_peers (ACE_ENV_SINGLE_ARG_DECL);

  // = The skeleton methods
  virtual void add_peer (Test::Peer_ptr peer
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  /// Protected Destructor, call _remove_ref() instead
  virtual ~Coordinator (void);

private:
  /// Store a reference to each peer
  Test::Peer_var *peers_;

  /// Current number of elements in the array
  size_t peer_count_;

  /// Array's capacity
  size_t peer_max_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* BIG_ONEWAYS_COORDINATOR_H */
