#include "Consumer_Router.h"
// $Id$

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

// Initialize the Router..

int
Consumer_Router::open (void *)
{
  ACE_ASSERT (this->is_reader ());
  char *argv[3];

  argv[0] = (char *) this->name ();
  argv[1] = options.consumer_file ();
  argv[2] = 0;

  if (this->init (1, &argv[1]) == -1)
    return -1;

  // Make this an active object.
  //   return this->activate (options.t_flags ());

  // Until that's done, return 1 to indicate that the object wasn't activated.
  return 1;
}

int
Consumer_Router::close (u_long)
{
  ACE_ASSERT (this->is_reader ());
  this->peer_map_.close ();
  this->msg_queue ()->deactivate();
  return 0;
}


// Handle incoming messages in a separate thread..

int
Consumer_Router::svc (void)
{
  ACE_Message_Block *mb = 0;

  ACE_ASSERT (this->is_reader ());

  if (options.debug ())
    ACE_DEBUG ((LM_DEBUG, "(%t) starting svc in %s\n", this->name ()));

  while (this->getq (mb) > 0)
    if (this->put_next (mb) == -1)
      ACE_ERROR_RETURN ((LM_ERROR, "(%t) put_next failed in %s\n", this->name ()), -1);

  return 0;
  // Note the implicit ACE_OS::thr_exit() via destructor.
}

// Send a MESSAGE_BLOCK to the supplier(s)..

int
Consumer_Router::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  ACE_ASSERT (this->is_reader ());

  if (mb->msg_type () == ACE_Message_Block::MB_IOCTL)
    {
      this->control (mb);
      return this->put_next (mb);
    }
  else
{
//printf("consumer-Router is routing : send_peers\n");
    return this->send_peers (mb);
}
}

// Return information about the Client_Router ACE_Module..

int
Consumer_Router::info (char **strp, size_t length) const
{
  char	     buf[BUFSIZ];
  ACE_UPIPE_Addr  addr;
  const char *mod_name = this->name ();
  ACE_UPIPE_Acceptor &sa = (ACE_UPIPE_Acceptor &) *this->acceptor_;

  if (sa.get_local_addr (addr) == -1)
    return -1;

  ACE_OS::sprintf (buf, "%s\t /%s %s",
	     mod_name,  "upipe",
	     "# consumer router\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (mod_name)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, mod_name, length);
  return ACE_OS::strlen (mod_name);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Acceptor<Consumer_Handler, ACE_UPIPE_ACCEPTOR>;
template class Acceptor_Factory<Consumer_Handler, CONSUMER_KEY>;
template class Peer_Handler<CONSUMER_ROUTER, CONSUMER_KEY>;
template class Peer_Router<Consumer_Handler, CONSUMER_KEY>;
template class ACE_Map_Entry<CONSUMER_KEY, Consumer_Handler *>;
template class ACE_Map_Iterator<CONSUMER_KEY, Consumer_Handler *, ACE_RW_Mutex>;
template class ACE_Map_Manager<CONSUMER_KEY, Consumer_Handler *, ACE_RW_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Acceptor<Consumer_Handler, ACE_UPIPE_ACCEPTOR>
#pragma instantiate Acceptor_Factory<Consumer_Handler, CONSUMER_KEY>
#pragma instantiate Peer_Handler<CONSUMER_ROUTER, CONSUMER_KEY>
#pragma instantiate Peer_Router<Consumer_Handler, CONSUMER_KEY>
#pragma instantiate ACE_Map_Entry<CONSUMER_KEY, Consumer_Handler *>
#pragma instantiate ACE_Map_Iterator<CONSUMER_KEY, Consumer_Handler *, ACE_RW_Mutex>
#pragma instantiate ACE_Map_Manager<CONSUMER_KEY, Consumer_Handler *, ACE_RW_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */
