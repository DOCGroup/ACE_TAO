#include "ace/Log_Msg.h"
// @(#)Consumer_Router.cpp	1.1	10/18/96

#include "Consumer_Router.h"
#include "Options.h"

#if defined (ACE_HAS_THREADS)

typedef Acceptor_Factory<Consumer_Handler, CONSUMER_KEY> CONSUMER_FACTORY;

int
Consumer_Handler::open (void *a)
{
  CONSUMER_FACTORY *af = (CONSUMER_FACTORY *) a;
  this->router_task_ = af->router (); 
  return this->Peer_Handler<CONSUMER_ROUTER, CONSUMER_KEY>::open (a);
}

Consumer_Handler::Consumer_Handler (ACE_Thread_Manager *tm)
  : Peer_Handler<CONSUMER_ROUTER, CONSUMER_KEY> (tm)
{
}

// Create a new handler that will interact with a consumer and point
// its ROUTER_TASK_ data member to the CONSUMER_ROUTER.

Consumer_Router::Consumer_Router (ACE_Thread_Manager *tm)
  : CONSUMER_ROUTER (tm)
{
}

// Initialize the Router. 

int
Consumer_Router::open (void *)
{
  assert (this->is_reader ());

  char *argv[4];

  argv[0] = (char *) this->name ();
  argv[1] = "-p";
  argv[2] = options.consumer_port ();
  argv[3] = 0;

  if (this->init (2, &argv[1]) == -1)
    return -1;

  // Make this an active object.
  return this->activate (options.t_flags ());
}

int
Consumer_Router::close (u_long)
{
  assert (this->is_reader ());
  ACE_DEBUG ((LM_DEBUG, "(%t) closing Consumer_Router\n"));
  this->peer_map_.close ();

  // Inform the thread to shut down.
  this->msg_queue ()->deactivate ();
  return 0;
}

// Handle incoming messages in a separate thread. 

int 
Consumer_Router::svc (void)
{
  ACE_Thread_Control tc (this->thr_mgr ());
  ACE_Message_Block *mb = 0;

  assert (this->is_reader ());

  ACE_DEBUG ((LM_DEBUG, "(%t) starting svc in Consumer_Router\n"));

  while (this->getq (mb) >= 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Consumer_Router is routing via send_peers\n"));
      if (this->send_peers (mb) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, 
			   "(%t) send_peers failed in Consumer_Router\n"),
			  -1);
    }
  ACE_DEBUG ((LM_DEBUG, "(%t) stopping svc in Consumer_Router\n"));
  return 0;
  // Note the implicit ACE_OS::thr_exit() via destructor.
}

// Send a MESSAGE_BLOCK to the supplier(s). 

int 
Consumer_Router::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  assert (this->is_reader ());

  if (mb->msg_type () == ACE_Message_Block::MB_IOCTL)
    {
      this->control (mb);
      return this->put_next (mb);
    }
  else
    // Queue up the message, which will be processed by
    // Consumer_Router::svc().
    return this->putq (mb);
}

// Return information about the Client_Router ACE_Module. 

int
Consumer_Router::info (char **strp, size_t length) const
{
  char	     buf[BUFSIZ];
  ACE_INET_Addr  addr;
  const char *mod_name = this->name ();
  ACE_SOCK_Acceptor &sa = this->acceptor_->acceptor ();
  
  if (sa.get_local_addr (addr) == -1)
    return -1;
  
  ACE_OS::sprintf (buf, "%s\t %d/%s %s",
	     mod_name, addr.get_port_number (), "tcp",
	     "# consumer router\n");
  
  if (*strp == 0 && (*strp = ACE_OS::strdup (mod_name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, mod_name, length);
  return ACE_OS::strlen (mod_name);
}

#endif /* ACE_HAS_THREADS */
