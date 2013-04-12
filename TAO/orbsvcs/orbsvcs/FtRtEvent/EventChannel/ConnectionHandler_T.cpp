// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "ace/INET_Addr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <ACE_PEER_STREAM_1>
int ConnectionAcceptHandler<ACE_PEER_STREAM_2>::open (void * acceptor)
{

  ACE_TRACE("ConnectionAcceptHandler::open\n");
  ACE_INET_Addr addr;

  if (this->peer ().get_remote_addr (addr) == -1)
    return -1;

  reactor_ = static_cast<ACE_Service_Object*> (acceptor)->reactor();

  if (reactor_->register_handler (this,
    ACE_Event_Handler::READ_MASK) == -1)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
    "(%P|%t) can't register with reactor\n"),
    -1);

  ORBSVCS_DEBUG ((LM_DEBUG,
    "(%P|%t) connected with %s\n",
    addr.get_host_name ()));

  return 0;
}

template <ACE_PEER_STREAM_1>
void ConnectionAcceptHandler<ACE_PEER_STREAM_2>::destroy (void)
{
  // Remove ourselves from the reactor
  reactor_->remove_handler
    (this,
    ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

  // Shut down the connection to the client.
  this->peer ().close ();

  // Free our memory.
  delete this;
}

// If somebody doesn't like us, they will close() us.  Actually, if
//  our open() method returns -1, the Acceptor<> will invoke close()
//  on us for cleanup.
template <ACE_PEER_STREAM_1>
int ConnectionAcceptHandler<ACE_PEER_STREAM_2>::close (u_long flags)
{
  ACE_UNUSED_ARG (flags);

  this->destroy ();
  return 0;
}


template <ACE_PEER_STREAM_1>
int ConnectionAcceptHandler<ACE_PEER_STREAM_2>::handle_input (ACE_HANDLE)
{
  char buf[8];
  if (this->peer().recv(buf, sizeof(buf)))
    return -1;
  return 0;
}

// Clean ourselves up when handle_input() (or handle_timer()) returns -1

template <ACE_PEER_STREAM_1>
int ConnectionAcceptHandler<ACE_PEER_STREAM_2>::handle_close (ACE_HANDLE,
                                                        ACE_Reactor_Mask)
{
  this->destroy ();
  return 0;
}


template <ACE_PEER_STREAM_1>
int ConnectionDetectHandler<ACE_PEER_STREAM_2>::handle_close (ACE_HANDLE,
                                                        ACE_Reactor_Mask)
{
  ACE_TRACE("ConnectionDetectHandler::handle_close\n");
  this->close();
  return 0;
}

template <ACE_PEER_STREAM_1>
int ConnectionDetectHandler<ACE_PEER_STREAM_2>::close (u_long )
{
  if (listener_)
    listener_->connection_closed();
  delete this;
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
