/* -*- C++ -*- */
// $Id$

#if !defined (_LOGGING_ACCEPTOR_H)
#define _LOGGING_ACCEPTOR_H

#include "ace/SOCK_Acceptor.h"
#include "ace/Event_Handler.h"

#include "logger.h"

class Logging_Acceptor : public ACE_Event_Handler
{
  friend class Logging_Handler;
public:

  Logging_Acceptor ( const ACE_INET_Addr &addr) : peer_acceptor_(addr)
  {
  }

  ~Logging_Acceptor (void)
  {
    this->handle_close (ACE_INVALID_HANDLE, ACE_Event_Handler::READ_MASK);
  }


  virtual int open (const ACE_INET_Addr &addr)
  {
    if (this->peer_acceptor_.open (addr, 1) == -1)
      return -1;
    else
      return 0;
  }
	
private:

  virtual ACE_HANDLE get_handle (void) const
  { 
    return this->peer_acceptor_.get_handle (); 
  }

  virtual int handle_input (ACE_HANDLE)
  {
    Logging_Handler *svc_handler = new Logging_Handler;
	
    if (this->peer_acceptor_.accept (*svc_handler) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "%p", "accept failed"), -1);
    else if (svc_handler->open () == -1)
      svc_handler->close ();
	
    return 0;
  }

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask)
  {
    return this->peer_acceptor_.close ();
  }


  ACE_SOCK_Acceptor peer_acceptor_;     
};

#endif /* _LOGGING_ACCEPTOR_H */

