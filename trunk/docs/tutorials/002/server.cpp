
// $Id$


#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor.h"

ACE_Reactor * g_reactor;

static sig_atomic_t finished = 0;
extern "C" void handler (int) { finished = 1; }

// ----------------------------------------

class Logging_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{

public:

  Logging_Handler (void)
	{
	}

  virtual void destroy (void)
	{
	  g_reactor->cancel_timer (this);
	  this->peer ().close ();
	  delete this;
	}

  virtual int open (void *)
	{
	  ACE_INET_Addr addr;
	  
	  if (this->peer ().get_remote_addr (addr) == -1)
	    return -1;
	  else
	    {
	      ACE_OS::strncpy (this->peer_name_, addr.get_host_name (), MAXHOSTNAMELEN + 1);
	
	      if (g_reactor->register_handler(this, ACE_Event_Handler::READ_MASK) == -1)
		ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) can't register with reactor\n"), -1);

	      else if (g_reactor->schedule_timer (this, (const void *) this, ACE_Time_Value (2), ACE_Time_Value (2)) == -1)
		ACE_ERROR_RETURN ((LM_ERROR, "can'(%P|%t) t register with reactor\n"), -1);

	      else
	      	ACE_DEBUG ((LM_DEBUG, "(%P|%t) connected with %s\n", this->peer_name_));

	      return 0;
	    }
	}

  virtual int close (u_long)
	{
	  this->destroy ();
	  return 0;
	}

protected:

  virtual int handle_input (ACE_HANDLE)
        {
          char buf[128];
          memset(buf,0,sizeof(buf));
        
          switch( this->peer().recv(buf,sizeof buf) )
          {
          case -1:
            ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p bad read\n", "client logger"), -1);
          case 0:
            ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) closing log daemon (fd = %d)\n", this->get_handle ()), -1);
          default:
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) from client: %s",buf));
          }
          
          return 0;
        }

  virtual int handle_timeout (const ACE_Time_Value &tv, const void *arg)
	{
	  ACE_ASSERT (arg == this);
	  ACE_DEBUG ((LM_DEBUG, "(%P|%t) handling timeout from this = %u\n", this));
	  return 0;
	}

private:

  char peer_name_[MAXHOSTNAMELEN + 1];

};


typedef ACE_Acceptor <Logging_Handler, ACE_SOCK_ACCEPTOR> Logging_Acceptor;


static const u_short PORT = ACE_DEFAULT_SERVER_PORT;

int main (int argc, char *argv[])
{
  g_reactor = new ACE_Reactor;

  // Acceptor factory.
  Logging_Acceptor peer_acceptor;

  /*  Provide a reactor on the open.  This prevents ACE_Reactor::instance() from
      creating an unnecessary instance.
   */
  if (peer_acceptor.open (ACE_INET_Addr (PORT), g_reactor) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  /*  This is redundant because opening the acceptor with a reactor pointer will
      cause the acceptor to be registered with that reactor.
   */
  else if (g_reactor->register_handler (&peer_acceptor, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "registering service with ACE_Reactor\n"), -1);

  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);

  // Run forever, performing logging service.

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) starting up server logging daemon\n"));

  // Perform logging service until QUIT_HANDLER receives SIGINT.
  while ( !finished )
    g_reactor->handle_events ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) shutting down server logging daemon\n"));

  return 0;
}
