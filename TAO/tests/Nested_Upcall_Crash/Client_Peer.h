/**
 * @file Client_Peer.h
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#ifndef Client_Peer__h_
#define Client_Peer__h_

#include "TestS.h"

class Client_Peer
  : public POA_Test::Peer
{
public:
  explicit Client_Peer (CORBA::ORB_ptr orb);
  Client_Peer (Client_Peer const &) = delete;
  Client_Peer (Client_Peer &&) = delete;
  Client_Peer &operator= (Client_Peer const &) = delete;
  Client_Peer &operator= (Client_Peer &&) = delete;

  void callme (Test::Peer_ptr callback,
               CORBA::ULong max_depth,
               Test::Payload const &) override;
  void crash () override;
  void noop () override;

private:
  CORBA::ORB_var orb_ {};
};

#endif /* Client_Peer__h_ */
