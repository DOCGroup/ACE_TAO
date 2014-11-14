/**
 * @file Server_Peer.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#ifndef Server_Peer__h_
#define Server_Peer__h_

#include "TestS.h"

class Server_Peer
  : public POA_Test::Peer
{
public:
  Server_Peer (unsigned int seed,
               CORBA::ORB_ptr orb,
               CORBA::ULong payload_size);

  void callme(Test::Peer_ptr callback,
              CORBA::ULong max_depth,
              Test::Payload const & extra_data);
  void crash(void);
  void noop(void);

private:
  unsigned int seed_;

  CORBA::ORB_var orb_;

  CORBA::ULong payload_size_;
};

#endif /* Server_Peer__h_ */
