//
// $Id$
//

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
  Peer (CORBA::ORB_ptr orb);

  /// Destructor
  virtual ~Peer (void);

  // = The skeleton methods
  virtual Test::Session_ptr create_session (Test::Session_Control_ptr control,
                                            CORBA::ULong payload_size,
                                            CORBA::ULong thread_count,
                                            CORBA::ULong message_count,
                                            CORBA::ULong peer_count);
  virtual void shutdown (void);

private:
  /// Keep a pointer to the ORB to shutdown cleanly
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* BIG_ONEWAYS_PEER_H */
