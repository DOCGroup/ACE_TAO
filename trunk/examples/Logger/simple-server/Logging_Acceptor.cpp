// $Id$

#include "Logging_Acceptor.h"
#include "Logging_Handler.h"
#include "Reactor_Singleton.h"
#include "ace/WFMO_Reactor.h"

// Initialize peer_acceptor object.

int
Logging_Acceptor::open (const ACE_INET_Addr &addr)
{
  // Reuse addr if already in use.
  if (this->peer_acceptor_.open (addr, 1) == -1)
    return -1;
  else
    return 0;
}

// Default constructor.

Logging_Acceptor::Logging_Acceptor (void)
{
}

// Performs termination activities.

int
Logging_Acceptor::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  return this->peer_acceptor_.close ();
}

Logging_Acceptor::~Logging_Acceptor (void)
{
  this->handle_close (ACE_INVALID_HANDLE, 
		      ACE_Event_Handler::ACCEPT_MASK);
}

// Returns underlying device descriptor.

ACE_HANDLE
Logging_Acceptor::get_handle (void) const
{ 
  return this->peer_acceptor_.get_handle (); 
}

// Accepts connections from client and registers new object with the
// ACE_Reactor.

int
Logging_Acceptor::handle_input (ACE_HANDLE)
{
  Logging_Handler *svc_handler;
  
  ACE_NEW_RETURN (svc_handler, Logging_Handler, -1);

  // Accept the connection from a client client daemon.

  int reset_new_handle = 0;
#if defined (ACE_WIN32)
  // Try to find out if the implementation of the reactor that we are
  // using is the WFMO_Reactor. If so we need to reset the event
  // association for the newly created handle. This is because the
  // newly created handle will inherit the properties of the listen
  // handle, including its event associations.
  reset_new_handle = this->reactor ()->reset_new_handle ();
#endif /* ACE_WIN32 */
  
  if (this->peer_acceptor_.accept (svc_handler->peer (), // stream
                                   0, // remote address
                                   0, // timeout
                                   1, // restart
                                   reset_new_handle  // reset new handler
                                   ) == -1
      || svc_handler->open () == -1)
    {
      svc_handler->close ();
      ACE_ERROR_RETURN ((LM_ERROR, "%p", "accept/open failed"), -1);
    }

  return 0;
}
