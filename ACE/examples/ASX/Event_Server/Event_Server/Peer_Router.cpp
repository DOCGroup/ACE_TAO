// $Id$

#if !defined (_PEER_ROUTER_C)
#define _PEER_ROUTER_C

#include "ace/Service_Config.h"
#include "ace/Get_Opt.h"
#include "Options.h"
#include "Peer_Router.h"



// Send the <ACE_Message_Block> to all the peers.  Note that in a
// "real" application this logic would most likely be more selective,
// i.e., it would actually do "routing" based on addressing
// information passed in the <ACE_Message_Block>.

int
Peer_Router_Context::send_peers (ACE_Message_Block *mb)
{
  PEER_ITERATOR map_iter = this->peer_map_;
  int bytes = 0;
  int iterations = 0;

  // Skip past the header and get the message to send.
  ACE_Message_Block *data_block = mb->cont ();

  // Use an iterator to "multicast" the data to *all* the registered
  // peers.  Note that this doesn't really multicast, it just makes a
  // "logical" copy of the <ACE_Message_Block> and enqueues it in the
  // appropriate <Peer_Handler> corresponding to each peer.  Note that
  // a "real" application would probably "route" the data to a subset
  // of connected peers here, rather than send it to all the peers.

  for (PEER_ENTRY *ss = 0;
       map_iter.next (ss) != 0;
       map_iter.advance ())
    {
      if (Options::instance ()->debug ())
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) sending to peer via handle %d\n"),
                    ss->ext_id_));

      iterations++;

      // Increment reference count before sending since the
      // <Peer_Handler> might be running in its own thread of control.
      bytes += ss->int_id_->put (data_block->duplicate ());
    }

  mb->release ();
  return bytes == 0 ? 0 : bytes / iterations;
}

// Remove the <Peer_Handler> from the peer connection map.

int
Peer_Router_Context::unbind_peer (ROUTING_KEY key)
{
  return this->peer_map_.unbind (key);
}

// Add the <Peer_Handler> to the peer connection map.

int
Peer_Router_Context::bind_peer (ROUTING_KEY key,
                                Peer_Handler *peer_handler)
{
  return this->peer_map_.bind (key, peer_handler);
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

Peer_Router_Context::Peer_Router_Context (u_short port)
  : reference_count_ (0)
{
  // Initialize the Acceptor's "listen-mode" socket.
  ACE_INET_Addr endpoint (port);
  if (this->open (endpoint) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Acceptor::open")));

  // Initialize the connection map.
  else if (this->peer_map_.open () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("Map_Manager::open")));
  else
    {
      ACE_INET_Addr addr;

      if (this->acceptor ().get_local_addr (addr) != -1)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) initializing %C on port = %d, handle = %d, this = %u\n"),
                    addr.get_port_number () == Options::instance ()->supplier_port ()
                      ? "Supplier_Handler" : "Consumer_Handler",
                    addr.get_port_number (),
                    this->acceptor().get_handle (),
                    this));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("get_local_addr")));
    }
}

Peer_Router_Context::~Peer_Router_Context (void)
{
  // Free up the handle and close down the listening socket.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) closing down Peer_Router_Context\n")));

  // Close down the Acceptor and take ourselves out of the Reactor.
  this->handle_close ();

  PEER_ITERATOR map_iter = this->peer_map_;

  // Make sure to take all the handles out of the map to avoid
  // "resource leaks."

  for (PEER_ENTRY *ss = 0;
       map_iter.next (ss) != 0;
       map_iter.advance ())
    {
      if (Options::instance ()->debug ())
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) closing down peer on handle %d\n"),
                    ss->ext_id_));

      if (ACE_Reactor::instance ()->remove_handler(ss->ext_id_,
                                                   ACE_Event_Handler::READ_MASK) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%t) p\n"),
                    ACE_TEXT ("remove_handle")));
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

// Factory Method that creates a new <Peer_Handler> for each
// connection.

int
Peer_Router_Context::make_svc_handler (Peer_Handler *&sh)
{
  ACE_NEW_RETURN (sh,
                  Peer_Handler (this),
                  -1);
  return 0;
}

Peer_Handler::Peer_Handler (Peer_Router_Context *prc)
  : peer_router_context_ (prc)
{
}

// Send output to a peer.  Note that this implementation "blocks" if
// flow control occurs.  This is undesirable for "real" applications.
// The best way around this is to make the <Peer_Handler> an Active
// Object, e.g., as done in the $ACE_ROOT/apps/Gateway/Gateway
// application.

int
Peer_Handler::put (ACE_Message_Block *mb,
                   ACE_Time_Value *tv)
{
#if 0
  // If we're running as Active Objects just enqueue the message here.
  return this->putq (mb, tv);
#else
  ACE_UNUSED_ARG (tv);

  int result = this->peer ().send_n (mb->rd_ptr (),
                                     mb->length ());
  // Release the memory.
  mb->release ();

  return result;
#endif /* 0 */
}

// Initialize a newly connected handler.

int
Peer_Handler::open (void *)
{
  ACE_TCHAR buf[BUFSIZ], *p = buf;

  if (this->peer_router_context_->peer_router ()->info (&p,
                                                        sizeof buf) != -1)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) creating handler for %s, handle = %d\n"),
                buf,
                this->get_handle ()));
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("info")),
                      -1);
#if 0
  // If we're running as an Active Object activate the Peer_Handler
  // here.
  if (this->activate (Options::instance ()->t_flags ()) == -1)
     ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("%p\n"),
                        ACE_TEXT ("activation of thread failed")),
                       -1);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Peer_Handler::open registering with Reactor for handle_input\n")));
#else

  // Register with the Reactor to receive messages from our Peer.
  if (ACE_Reactor::instance ()->register_handler
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("register_handler")),
                      -1);
#endif /* 0 */

  // Insert outselves into the routing map.
  else if (this->peer_router_context_->bind_peer (this->get_handle (),
                                                  this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("bind_peer")),
                      -1);
  else
    return 0;
}

// Receive a message from a Peer.

int
Peer_Handler::handle_input (ACE_HANDLE h)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) input arrived on handle %d\n"),
              h));

  ACE_Message_Block *db;

  ACE_NEW_RETURN (db, ACE_Message_Block (BUFSIZ), -1);

  ACE_Message_Block *hb = new ACE_Message_Block (sizeof (ROUTING_KEY),
                                                 ACE_Message_Block::MB_PROTO, db);
  // Check for memory failures.
  if (hb == 0)
    {
      db->release ();
      errno = ENOMEM;
      return -1;
    }

  ssize_t n = this->peer ().recv (db->rd_ptr (),
                                  db->size ());

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p"),
                       ACE_TEXT ("recv failed")),
                      -1);
  else if (n == 0) // Client has closed down the connection.
    {
      if (this->peer_router_context_->unbind_peer (this->get_handle ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p"),
                           ACE_TEXT ("unbind failed")),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) shutting down handle %d\n"), h));
      // Instruct the <ACE_Reactor> to deregister us by returning -1.
      return -1;
    }
  else
    {
      // Transform incoming buffer into an <ACE_Message_Block>.

      // First, increment the write pointer to the end of the newly
      // read data block.
      db->wr_ptr (n);

      // Second, copy the "address" into the header block.  Note that
      // for this implementation the HANDLE we receive the message on
      // is considered the "address."  A "real" application would want
      // to do something more sophisticated.
      *(ACE_HANDLE *) hb->rd_ptr () = this->get_handle ();

      // Third, update the write pointer in the header block.
      hb->wr_ptr (sizeof (ACE_HANDLE));

      // Finally, pass the message through the stream.  Note that we
      // use <Task::put> here because this gives the method at *our*
      // level in the stream a chance to do something with the message
      // before it is sent up the other side.  For instance, if we
      // receive messages in the <Supplier_Router>, it will just call
      // <put_next> and send them up the stream to the
      // <Consumer_Router> (which broadcasts them to consumers).
      // However, if we receive messages in the <Consumer_Router>, it
      // could reply to the Consumer with an error since it's not
      // correct for Consumers to send messages (we don't do this in
      // the current implementation, but it could be done in a "real"
      // application).

      if (this->peer_router_context_->peer_router ()->put (hb) == -1)
        return -1;
      else
        return 0;
    }
}

Peer_Router::Peer_Router (Peer_Router_Context *prc)
  : peer_router_context_ (prc)
{
}

Peer_Router_Context *
Peer_Router::context (void) const
{
  return this->peer_router_context_;
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
  ACE_Message_Block *db, *hb;

  // Do an endless loop
  for (;;)
    {
      db = new Message_Block (BUFSIZ);
      hb = new Message_Block (sizeof (ROUTING_KEY),
                              Message_Block::MB_PROTO,
                              db);

      ssize_t n = this->peer_.recv (db->rd_ptr (), db->size ());

      if (n == -1)
        LM_ERROR_RETURN ((LOG_ERROR,
                          ACE_TEXT ("%p"),
                          ACE_TEXT ("recv failed")),
                         -1);
      else if (n == 0) // Client has closed down the connection.
        {
          if (this->peer_router_context_->peer_router ()->unbind_peer (this->get_handle ()) == -1)
            LM_ERROR_RETURN ((LOG_ERROR,
                              ACE_TEXT ("%p"),
                              ACE_TEXT ("unbind failed")),
                             -1);
            LM_DEBUG ((LOG_DEBUG,
                     ACE_TEXT ("(%t) shutting down\n")));

          // We do not need to be deregistered by reactor
          // as we were not registered at all.
          return -1;
        }
      else
        {
          // Transform incoming buffer into a Message.
          db->wr_ptr (n);
          *(long *) hb->rd_ptr () = this->get_handle (); // Structure assignment.
          hb->wr_ptr (sizeof (long));

          // Pass the message to the stream.
          if (this->peer_router_context_->peer_router ()->reply (hb) == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%t) %p\n"),
                               ACE_TEXT ("Peer_Handler.svc : peer_router->reply failed")),
                              -1);
        }
    }
  return 0;
}
#endif /* 0 */
#endif /* _PEER_ROUTER_C */

