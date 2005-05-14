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
  , public PortableServer::RefCountServantBase
{
public:
  Client_Peer (CORBA::ORB_ptr orb);

  void callme(Test::Peer_ptr callback,
              CORBA::ULong max_depth,
              Test::Payload const &
              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));
  void crash(ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));
  void noop(ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
};

#endif /* Client_Peer__h_ */
