
// $Id$


#if !defined (_CLIENT_HANDLER_H)
#define _CLIENT_HANDLER_H

#include "ace/Event_Handler.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"

class Logging_Handler : public ACE_Event_Handler
{
public:
  Logging_Handler (void)
	{
	}

  virtual int open (void)
	{
	      if (g_reactor-> register_handler (this, ACE_Event_Handler::READ_MASK) == -1)
		ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) can't register with reactor\n"), -1);
	      return 0;
	}

  virtual int close (void)
	{
	  return this->handle_close (ACE_INVALID_HANDLE, ACE_Event_Handler::RWE_MASK);
	}

  operator ACE_SOCK_Stream &()
	{
	  return this->cli_stream_;
	}

protected:
  virtual ACE_HANDLE get_handle (void) const
	{
	  return this->cli_stream_.get_handle ();
	}

  virtual int handle_input (ACE_HANDLE)
	{
	  char buf[128];
	  memset(buf,0,sizeof(buf));
		
	  switch( this->cli_stream_.recv(buf,sizeof buf) )
	  {
	  case -1:
	    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p bad read\n", "client logger"), -1);
	  case 0:
	    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) closing log daemon (fd = %d)\n",
				this->get_handle ()), -1);
	  default:
	    ACE_DEBUG ((LM_DEBUG, "(%P|%t) from client: %s",buf));
	  }
	
	  return 0;
	}

  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask)
	{
	  delete this;		
	  return 0;
	}

  virtual ~Logging_Handler (void)
	{
	  g_reactor-> cancel_timer (this);
	  this->cli_stream_.close ();
	}

private:
  ACE_SOCK_Stream cli_stream_; 
};

#endif /* _CLIENT_HANDLER_H */

