// $Id$

#if !defined (_PEER_ROUTER_C)
#define _PEER_ROUTER_C

#include "ace/Service_Config.h"
#include "ace/Get_Opt.h"
#include "Options.h"
#include "Peer_Router.h"

int
Peer_Router_Context::send_peers (ACE_Message_Block *mb)
{
  PEER_ITERATOR map_iter = this->peer_map_;
  int bytes = 0;
  int iterations = 0;

  // Skip past the header and get the message to send.
  ACE_Message_Block *data_block = mb->cont ();
  
  // "Multicast" the data to *all* the registered peers.

  for (PEER_ENTRY *ss = 0;
       map_iter.next (ss) != 0;
       map_iter.advance ())
    {
      if (Options::instance ()->debug ())
	ACE_DEBUG ((LM_DEBUG, 
		    "(%t) sending to peer via handle %d\n", 
		    ss->ext_id_));
      iterations++;
      // Increment reference count before sending since the
      // Peer_Handler might be running in its own thread of control.
      bytes += ss->int_id_->put (data_block->duplicate ()); 
    }

  mb->release ();
  return bytes == 0 ? 0 : bytes / iterations;
}

int
Peer_Router_Context::unbind_peer (ROUTING_KEY key)
{
  return this->peer_map_.unbind (key);
}

void
Peer_Router_Context::duplicate (void)
{
  this->reference_count_++;
}

void
Peer_Router_Context::release (void)
{
  ACE_ASSERT (this->reference_count_ > 0);
  this->reference_count_--;

  if (this->reference_count_ == 0)
    delete this;
}

int
Peer_Router_Context::bind_peer (ROUTING_KEY key, 
				Peer_Handler *peer_handler)
{
  return this->peer_map_.bind (key, peer_handler);
}

Peer_Router_Context::Peer_Router_Context (u_short port)
  : reference_count_ (0)
{
  // Perform initializations.

  if (this->open (ACE_INET_Addr (port)) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "Acceptor::open"));

  else if (this->peer_map_.open () == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "Map_Manager::open"));

  else
    {
      ACE_INET_Addr addr;
      
      if (this->acceptor().get_local_addr (addr) != -1)
	ACE_DEBUG ((LM_DEBUG, 
		    "(%t) initializing %s on port = %d, handle = %d, this = %u\n", 
		    addr.get_port_number () == Options::instance ()->supplier_port () ? 
		    "Supplier_Handler" : "Consumer_Handler",
		    addr.get_port_number (),
		    this->acceptor().get_handle (), 
		    this));
      else
	ACE_ERROR ((LM_ERROR, 
		    "%p\n", "get_local_addr"));
    }
}

Peer_Router_Context::~Peer_Router_Context (void)
{
  // Free up the handle and close down the listening socket.
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) closing down Peer_Router_Context\n"));

  // Close down the Acceptor and take ourselves out of the Reactor.
  this->handle_close ();

  PEER_ITERATOR map_iter = this->peer_map_;

  // Make sure to take all the handles out of the map.

  for (PEER_ENTRY *ss = 0;
       map_iter.next (ss) != 0;
       map_iter.advance ())
    {
      if (Options::instance ()->debug ())
	ACE_DEBUG ((LM_DEBUG, 
		    "(%t) closing down peer on handle %d\n", 
		    ss->ext_id_));

      if (ACE_Reactor::instance()->remove_handler
	  (ss->ext_id_, ACE_Event_Handler::READ_MASK) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) p\n", "remove_handle"));
    }

  // Close down the map.
  this->peer_map_.close ();
}

Peer_Router *
Peer_Router_Context::peer_router (void)
{
  return this->peer_router_;
}

void
Peer_Router_Context::peer_router (Peer_Router *pr)
{
  this->peer_router_ = pr;
}

int
Peer_Router_Context::make_svc_handler (Peer_Handler *&sh)
{ 
  ACE_NEW_RETURN (sh, Peer_Handler (this), -1);
  return 0;
}

Peer_Handler::Peer_Handler (Peer_Router_Context *prc)
  : prc_ (prc)
{
}

#if 0

// Right now, Peer_Handlers are purely Reactive, i.e., they all run in
// a single thread of control.  It would be easy to make them Active
// Objects by calling activate() in Peer_Handler::open(), making
// Peer_Handler::put() enqueue each message on the message queue, and
// (3) then running the following svc() routine to route each message
// to its final destination within a separate thread.  Note that we'd
// want to move the svc() call up to the Consumer_Router and
// Supplier_Router level in order to get the right level of control
// for input and output.

Peer_Handler::svc (void)
{
  ACE_Thread_Control thread_control (tm);

  ACE_Message_Block *db, *hb;
  int n;

  // Do an endless loop
  for (;;)
    {
      db = new Message_Block (BUFSIZ);
      hb = new Message_Block (sizeof (ROUTING_KEY), Message_Block::MB_PROTO, db);
   
      if ((n = this->peer_.recv (db->rd_ptr (), db->size ())) == -1)
	LM_ERROR_RETURN ((LOG_ERROR, "%p", "recv failed"), -1);    
      else if (n == 0) // Client has closed down the connection.
	{
	  if (this->prc_->peer_router ()->unbind_peer (this->get_handle ()) == -1)
	    LM_ERROR_RETURN ((LOG_ERROR, "%p", "unbind failed"), -1);
	  LM_DEBUG ((LOG_DEBUG, "(%t) shutting down \n"));
	  return -1; // We do not need to be deregistered by reactor
	  // as we were not registered at all
	}
      else 
	// Transform incoming buffer into a Message and pass
	// downstream.
	{
	  db->wr_ptr (n);
	  *(long *) hb->rd_ptr () = this->get_handle (); // Structure assignment.
	  hb->wr_ptr (sizeof (long));

	  if (this->prc_->peer_router ()->reply (hb) == -1)
	    {
	      cout << "Peer_Handler.svc : peer_router->reply failed" << endl ;
	      return -1;
	    }
	}
    }
  return 0;
}
#endif

int
Peer_Handler::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
#if 0
  // If we're running as Active Objects just enqueue the message here.
  return this->putq (mb, tv);
#else
  ACE_UNUSED_ARG (tv);

  int result = 0;

  result = this->peer ().send_n (mb->rd_ptr (), 
				 mb->length ());
  // Release the memory.
  mb->release ();
  return result;
#endif
}

// Initialize a newly connected handler.

int
Peer_Handler::open (void *)
{
  char buf[BUFSIZ], *p = buf;

  if (this->prc_->peer_router ()->info (&p, sizeof buf) != -1)
    ACE_DEBUG ((LM_DEBUG, "(%t) creating handler for %s, handle = %d\n",
	       buf, this->get_handle ()));
  else
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "info"), -1);
#if 0
  // If we're running as an Active Object activate the Peer_Handler
  // here.
  if (this->activate (Options::instance ()->t_flags ()) == -1)
     ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "activation of thread failed"), -1);
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) Peer_Handler::open registering with Reactor for handle_input\n"));
#else
  // Register with the Reactor to receive messages from our Peer.
  if (ACE_Reactor::instance()->register_handler 
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_handler"), -1);
#endif

  // Insert outselves into the routing map.
  else if (this->prc_->bind_peer (this->get_handle (), this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "bind_peer"), -1);

  // Turn off non-blocking I/O in case it was turned on.
  else if (this->peer ().disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "disable non-blocking I/O"), -1);
  else
    return 0;
}

// Receive a message from a Peer.

int
Peer_Handler::handle_input (ACE_HANDLE h)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) input arrived on handle %d\n", h));

  ACE_Message_Block *db = new ACE_Message_Block (BUFSIZ);
  ACE_Message_Block *hb = new ACE_Message_Block (sizeof (ROUTING_KEY),
						 ACE_Message_Block::MB_PROTO, db);

  // Check for memory failures.
  if (db == 0 || hb == 0)
    {
      hb->release ();
      db->release ();
      errno = ENOMEM;
      return -1;
    }

  ssize_t n = this->peer ().recv (db->rd_ptr (), db->size ());

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p", "recv failed"), -1);    
  else if (n == 0) // Client has closed down the connection.
    {
      if (this->prc_->unbind_peer (this->get_handle ()) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p", "unbind failed"), -1);

      ACE_DEBUG ((LM_DEBUG, "(%t) shutting down handle %d\n", h));
      return -1; // Instruct the ACE_Reactor to deregister us by returning -1.
    }
  else 
    {
      // Transform incoming buffer into a Message.

      // First, increment the write pointer to the end of the newly
      // read data block.
      db->wr_ptr (n);

      // Second, copy the "address" into the header block.
      *(ACE_HANDLE *) hb->rd_ptr () = this->get_handle (); 

      // Third, update the write pointer in the header block.
      hb->wr_ptr (sizeof (ACE_HANDLE));

      // Finally, pass the message through the stream.  Note that we
      // use <Task::put> here because this gives the method at *our*
      // level in the stream a chance to do something with the message
      // before it is sent up the other side.  For instance, if we
      // receive messages in the Supplier_Router, it will just call
      // <put_next> and send them up the stream to the Consumer_Router
      // (which broadcasts them to consumers).  However, if we receive
      // messages in the Consumer_Router, it will reply to the
      // Consumer with an error since it's not correct for Consumers
      // to send messages.
      return this->prc_->peer_router ()->put (hb) == -1 ? -1 : 0;
    }
}

Peer_Router::Peer_Router (Peer_Router_Context *prc)
  : prc_ (prc)
{

}

Peer_Router_Context *
Peer_Router::context (void) const
{
  return this->prc_;
}

int
Peer_Router::control (ACE_Message_Block *mb)
{
  ACE_IO_Cntl_Msg *ioc = (ACE_IO_Cntl_Msg *) mb->rd_ptr ();
  ACE_IO_Cntl_Msg::ACE_IO_Cntl_Cmds command;

  switch (command = ioc->cmd ())
    {
    case ACE_IO_Cntl_Msg::SET_LWM:
    case ACE_IO_Cntl_Msg::SET_HWM:
      this->water_marks (command, *(size_t *) mb->cont ()->rd_ptr ());
      break;
    default:
      return -1;
    }
  return 0;
}

#endif /* _PEER_ROUTER_C */

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Map_Entry<ROUTING_KEY, Peer_Handler *>;
template class ACE_Map_Iterator<ROUTING_KEY, Peer_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Manager<ROUTING_KEY, Peer_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
