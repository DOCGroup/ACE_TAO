/**
 * @file Server_Peer.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Server_Peer.h"
#include "tao/debug.h"

Server_Peer::Server_Peer (ACE_RANDR_TYPE seed,
                          CORBA::ORB_ptr orb,
                          CORBA::ULong payload_size)
  : seed_ (seed)
  , orb_ (CORBA::ORB::_duplicate(orb))
  , payload_size_ (payload_size)
{
}

void
Server_Peer::callme(Test::Peer_ptr callback,
                    CORBA::ULong max_depth,
                    Test::Payload const &)
{
  int r = ACE_OS::rand_r(this->seed_) % 50;

  if(TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Server_Peer::callme[%d], r = %d\n",
                  max_depth, r));
    }

  if(r == 0)
  {
    try
    {
      callback->crash();
    }
    catch (const CORBA::Exception&)
    {
    }

    // orb_->shutdown (0);
  }
  else if (max_depth > 0)
  {
    Test::Peer_var me = this->_this ();

    Test::Payload extra_data(this->payload_size_);
    extra_data.length(this->payload_size_);
    callback->callme(me.in(),
                     max_depth - 1,
                     extra_data);
  }
}

void
Server_Peer::crash(void)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
Server_Peer::noop(void)
{
}
