#include "Supplier_Router.h"
// $Id$

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

// Create a new router and associate it with the REACTOR parameter..

Supplier_Router::Supplier_Router (ACE_Thread_Manager *tm)
  : SUPPLIER_ROUTER (tm)
{
}

// Handle incoming messages in a separate thread..

int 
Supplier_Router::svc (void)
{
  ACE_ASSERT (this->is_writer ());

  ACE_Thread_Control tc (this->thr_mgr ());
  ACE_Message_Block *message_block = 0;

  if (options.debug ())
    ACE_DEBUG ((LM_DEBUG, "(%t) starting svc in %s\n", this->name ()));

  while (this->getq (message_block) > 0)
  {
    if (this->put_next (message_block) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "(%t) put_next failed in %s\n", this->name ()), -1);
  }

  return 0;
  // Note the implicit ACE_OS::thr_exit() via ACE_Thread_Control's destructor.
}

// Initialize the Router..

int
Supplier_Router::open (void *)
{
  ACE_ASSERT (this->is_writer ());
  char *argv[3];

  argv[0] = (char *) this->name ();
  argv[1] = options.supplier_file ();
  argv[2] = 0;

  if (this->init (1, &argv[1]) == -1)
    return -1;

  // Make this an active object.
  //   return this->activate (options.t_flags ());

  // Until that's done, return 1 to indicate that the object wasn't activated.
  return 1;
}

// Close down the router..

int
Supplier_Router::close (u_long)
{
  ACE_ASSERT (this->is_writer ());
  this->peer_map_.close ();
  this->msg_queue ()->deactivate();
  return 0;
}

// Send a MESSAGE_BLOCK to the supplier(s)..

int 
Supplier_Router::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  ACE_ASSERT (this->is_writer ());

  if (mb->msg_type () == ACE_Message_Block::MB_IOCTL)
    {
      this->control (mb);
      return this->put_next (mb);
    }
  else
    {
//printf("supplier-Router is routing: send_peers\n");
      return this->send_peers (mb);
    }
}

// Return information about the Supplier_Router ACE_Module..

int 
Supplier_Router::info (char **strp, size_t length) const
{
  char       buf[BUFSIZ];
  ACE_UPIPE_Addr  addr;
  const char *mod_name = this->name ();
  ACE_UPIPE_Acceptor &sa = (ACE_UPIPE_Acceptor &) *this->acceptor_;

  if (sa.get_local_addr (addr) == -1)
    return -1;
  
  ACE_OS::sprintf (buf, "%s\t %s/ %s",
	     mod_name,  "upipe",
	     "# supplier router\n");
  
  if (*strp == 0 && (*strp = ACE_OS::strdup (mod_name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, mod_name, length);
  return ACE_OS::strlen (mod_name);
}

#endif /* ACE_HAS_THREADS */


#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class Acceptor_Factory<Supplier_Handler, SUPPLIER_KEY>;
template class ACE_Acceptor<Supplier_Handler, ACE_UPIPE_ACCEPTOR>;
template class ACE_Guard<ACE_RW_Mutex>;
template class ACE_Map_Iterator<SUPPLIER_KEY, Supplier_Handler *, ACE_RW_Mutex>;
template class ACE_Map_Manager<SUPPLIER_KEY, Supplier_Handler *, ACE_RW_Mutex>;
template class ACE_Read_Guard<ACE_RW_Mutex>;
template class ACE_Write_Guard<ACE_RW_Mutex>;
template class ACE_Svc_Handler<ACE_UPIPE_STREAM, ACE_MT_SYNCH>;
template class ACE_TSS<ACE_Dynamic>;
template class Peer_Handler<SUPPLIER_ROUTER, SUPPLIER_KEY>;
template class Peer_Router<Supplier_Handler, SUPPLIER_KEY>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
