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
  , public PortableServer::RefCountServantBase
{
public:
  Server_Peer (ACE_RANDR_TYPE seed,
               CORBA::ORB_ptr orb,
               CORBA::ULong payload_size);

  void callme(Test::Peer_ptr callback,
              CORBA::ULong max_depth,
              Test::Payload const & extra_data
              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));
  void crash(ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));
  void noop(ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC((CORBA::SystemException));

private:
  ACE_RANDR_TYPE seed_;

  CORBA::ORB_var orb_;

  CORBA::ULong payload_size_;
};

#endif /* Server_Peer__h_ */
