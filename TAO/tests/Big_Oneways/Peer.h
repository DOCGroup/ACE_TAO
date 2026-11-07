
#ifndef BIG_ONEWAYS_PEER_H
#define BIG_ONEWAYS_PEER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Peer interface
class Peer
  : public virtual POA_Test::Peer
{
public:
  /// Constructor
  explicit Peer (CORBA::ORB_ptr orb);
  Peer (Peer const &) = delete;
  Peer (Peer &&) = delete;
  Peer &operator= (Peer const &) = delete;
  Peer &operator= (Peer &&) = delete;

  /// Destructor
  ~Peer () override = default;

  // = The skeleton methods
  Test::Session_ptr create_session (Test::Session_Control_ptr control,
                                    CORBA::ULong payload_size,
                                    CORBA::ULong thread_count,
                                    CORBA::ULong message_count,
                                    CORBA::ULong peer_count) override;
  void shutdown () override;

private:
  /// Keep a pointer to the ORB to shutdown cleanly
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* BIG_ONEWAYS_PEER_H */
