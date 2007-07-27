/**
 * @file Client_Peer.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#ifndef Client_Peer__h_
#define Client_Peer__h_

#include "TestS.h"

class Client_Peer
  : public POA_Test::Peer
{
public:
  Client_Peer (CORBA::ORB_ptr orb);

  void callme(Test::Peer_ptr callback,
              CORBA::ULong max_depth,
              Test::Payload const &);
  void crash(void);
  void noop(void);

private:
  CORBA::ORB_var orb_;
};

#endif /* Client_Peer__h_ */
