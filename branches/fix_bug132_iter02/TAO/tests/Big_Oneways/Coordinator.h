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

  /// Destructor
  virtual ~Coordinator (void);

  /// Check if all the peers have registered already
  int has_all_peers (void) const;

  /// Check a session on each peer
  void create_session_list (Test::Session_Control_ptr session_control,
                            Test::Session_List &session_list,
                            CORBA::Environment &ACE_TRY_ENV);

  /// Shutdown all the peers
  void shutdown_all_peers (CORBA::Environment &ACE_TRY_ENV);

  // = The skeleton methods
  virtual void add_peer (Test::Peer_ptr peer,
                         CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

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
