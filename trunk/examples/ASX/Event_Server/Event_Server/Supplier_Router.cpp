#include "Supplier_Router.h"
// @(#)Supplier_Router.cpp	1.1	10/18/96

#include "Options.h"

#if defined (ACE_HAS_THREADS)

typedef Acceptor_Factory<Supplier_Handler, SUPPLIER_KEY> SUPPLIER_FACTORY;

int
Supplier_Handler::open (void *a)
{
  SUPPLIER_FACTORY *af = (SUPPLIER_FACTORY *) a;
  this->router_task_ = af->router (); 
  return this->Peer_Handler<SUPPLIER_ROUTER, SUPPLIER_KEY>::open (a);
}

Supplier_Handler::Supplier_Handler (ACE_Thread_Manager *tm)
  : Peer_Handler<SUPPLIER_ROUTER, SUPPLIER_KEY> (tm)
{
}

// Create a new router and associate it with the REACTOR parameter. 

Supplier_Router::Supplier_Router (ACE_Thread_Manager *tm)
  : SUPPLIER_ROUTER (tm)
{
}

// Handle incoming messages in a separate thread. 

int 
Supplier_Router::svc (void)
{
  assert (this->is_writer ());

  ACE_Thread_Control tc (this->thr_mgr ());
  ACE_Message_Block *mb = 0;

  ACE_DEBUG ((LM_DEBUG, "(%t) starting svc in Supplier_Router\n"));

  while (this->getq (mb) >= 0)
    {
      ACE_DEBUG ((LM_DEBUG, "Supplier_Router is routing via send_peers\n"));
      if (this->send_peers (mb) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, 
			   "(%t) send_peers failed in Supplier_Router\n"),
			   -1);
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) stopping svc in Supplier_Router\n"));
  return 0;
  // Note the implicit ACE_OS::thr_exit() via ACE_Thread_Control's
  // destructor.
}

// Initialize the Router. 

int
Supplier_Router::open (void *)
{
  assert (this->is_writer ());

  char *argv[4];

  argv[0] = (char *) this->name ();
  argv[1] = "-p";
  argv[2] = options.supplier_port ();
  argv[3] = 0;

  if (this->init (2, &argv[1]) == -1)
    return -1;

  // Make this an active object.
  return this->activate (options.t_flags ());
}

// Close down the router. 

int
Supplier_Router::close (u_long)
{
  assert (this->is_writer ());
  ACE_DEBUG ((LM_DEBUG, "(%t) closing Supplier_Router\n"));
  this->peer_map_.close ();

  // Inform the thread to shut down.
  this->msg_queue ()->deactivate ();
  return 0;
}

// Send a MESSAGE_BLOCK to the supplier(s). 

int 
Supplier_Router::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  assert (this->is_writer ());

  if (mb->msg_type () == ACE_Message_Block::MB_IOCTL)
    {
      this->control (mb);
      return this->put_next (mb);
    }
  else
    // Queue up the message, which will be processed by
    // Supplier_Router::svc().
    return this->putq (mb);
}

// Return information about the Supplier_Router ACE_Module. 

int 
Supplier_Router::info (char **strp, size_t length) const
{
  char       buf[BUFSIZ];
  ACE_INET_Addr  addr;
  const char *mod_name = this->name ();
  ACE_SOCK_Acceptor &sa = this->acceptor_->acceptor ();

  if (sa.get_local_addr (addr) == -1)
    return -1;
  
  ACE_OS::sprintf (buf, "%s\t %d/%s %s",
	     mod_name, addr.get_port_number (), "tcp",
	     "# supplier router\n");
  
  if (*strp == 0 && (*strp = ACE_OS::strdup (mod_name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, mod_name, length);
  return ACE_OS::strlen (mod_name);
}

#endif /* ACE_HAS_THREADS */
