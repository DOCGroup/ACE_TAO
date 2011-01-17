//
// $Id$
//

#ifndef BIG_ONEWAYS_COORDINATOR_H
#define BIG_ONEWAYS_COORDINATOR_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Coordinator interface
class Coordinator
  : public virtual POA_Test::Coordinator
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
                            Test::Session_List &session_list);

  /// Shutdown all the peers
  void shutdown_all_peers (void);

  // = The skeleton methods
  virtual void add_peer (Test::Peer_ptr peer);

protected:
  /// Protected destructor, call _remove_ref() to delete
  virtual ~Coordinator (void);

private:
  /// Store a reference to each peer
  Test::Peer_var *peers_;

  /// Current number of elements in the array
  size_t peer_count_;

  /// Array's capacity
  size_t peer_max_;
};

#include /**/ "ace/post.h"
#endif /* BIG_ONEWAYS_COORDINATOR_H */
