// $Id$

#define ACE_BUILD_SVC_DLL
#include "Event_Channel.h"
#include "Concrete_Connection_Handlers.h"

void
Connection_Handler::id (ACE_INT32 id)
{
  this->id_ = id;
}

ACE_INT32
Connection_Handler::id (void)
{
  return this->id_;
}

// The total number of bytes sent/received on this Proxy.

size_t
Connection_Handler::total_bytes (void)
{
  return this->total_bytes_;
}

void
Connection_Handler::total_bytes (size_t bytes)
{
  this->total_bytes_ += bytes;
}

Connection_Handler::Connection_Handler (void)
{
}

Connection_Handler::Connection_Handler (const Connection_Config_Info &pci)
  : remote_addr_ (pci.remote_port_, pci.host_),
    local_addr_ (pci.local_port_),
    id_ (pci.connection_id_),
    total_bytes_ (0),
    state_ (Connection_Handler::IDLE),
    timeout_ (1),
    max_timeout_ (pci.max_retry_timeout_),
    event_channel_ (pci.event_channel_)
{
  // Set the priority of the Proxy.
  this->priority (int (pci.priority_));
}

// Set the connection_role.

void
Connection_Handler::connection_role (char d)
{
  this->connection_role_ = d;
}

// Get the connection_role.

char
Connection_Handler::connection_role (void)
{
  return this->connection_role_;
}

// Sets the timeout delay.

void
Connection_Handler::timeout (int to)
{
  if (to > this->max_timeout_)
    to = this->max_timeout_;

  this->timeout_ = to;
}

// Re-calculate the current retry timeout delay using exponential
// backoff.  Returns the original timeout (i.e., before the
// re-calculation).

int
Connection_Handler::timeout (void)
{
  int old_timeout = this->timeout_;
  this->timeout_ *= 2;

  if (this->timeout_ > this->max_timeout_)
    this->timeout_ = this->max_timeout_;

  return old_timeout;
}

// Sets the max timeout delay.

void
Connection_Handler::max_timeout (int mto)
{
  this->max_timeout_ = mto;
}

// Gets the max timeout delay.

int
Connection_Handler::max_timeout (void)
{
  return this->max_timeout_;
}

// Restart connection asynchronously when timeout occurs.

int
Connection_Handler::handle_timeout (const ACE_Time_Value &,
			       const void *)
{
  ACE_DEBUG ((LM_DEBUG,
	     "(%t) attempting to reconnect Connection_Handler %d with timeout = %d\n",
             this->id (), this->timeout_));

  // Delegate the re-connection attempt to the Event Channel.
  this->event_channel_->initiate_connection_connection (this);

  return 0;
}

// Handle shutdown of the Connection_Handler object.

int
Connection_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
	      "(%t) shutting down %s Connection_Handler %d on handle %d\n",
	      this->connection_role () == 'C' ? "Consumer" : "Supplier",
	      this->id (), this->get_handle ()));

  // Restart the connection, if possible.
  return this->event_channel_->reinitiate_connection_connection (this);
}

// Set the state of the Proxy.

void
Connection_Handler::state (Connection_Handler::State s)
{
  this->state_ = s;
}

// Upcall from the <ACE_Acceptor> or <ACE_Connector> that delegates
// control to our Connection_Handler.

int
Connection_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s Connection_Handler's handle = %d\n",
	      this->connection_role () == 'C' ? "Consumer" : "Supplier",
	      this->peer ().get_handle ()));

  // Call back to the <Event_Channel> to complete our initialization.
  if (this->event_channel_->complete_connection_connection (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "complete_connection_connection"), -1);

  // Turn on non-blocking I/O.
  else if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enable"), -1);

  // Register ourselves to receive input events.
  else if (ACE_Reactor::instance ()->register_handler
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);
  else
    return 0;
}

// Return the current state of the Proxy.

Connection_Handler::State
Connection_Handler::state (void)
{
  return this->state_;
}

ACE_INET_Addr &
Connection_Handler::remote_addr (void)
{
  return this->remote_addr_;
}

ACE_INET_Addr &
Connection_Handler::local_addr (void)
{
  return this->local_addr_;
}

// Make the appropriate type of <Connection_Handler> (i.e.,
// <Consumer_Handler>, <Supplier_Handler>, <Thr_Consumer_Handler>, or
// <Thr_Supplier_Handler>).

Connection_Handler *
Connection_Handler_Factory::make_connection_handler (const Connection_Config_Info &pci)
{
  Connection_Handler *connection_handler = 0;

  // The next few lines of code are dependent on whether we are making
  // a threaded/reactive Supplier_Handler/Consumer_Handler.

  if (pci.connection_role_ == 'C') // Configure a Consumer_Handler.
    {
#if defined (ACE_HAS_THREADS)
      // Create a threaded Consumer_Handler.
      if (ACE_BIT_ENABLED (Options::instance ()->threading_strategy (),
			   Options::OUTPUT_MT))
	ACE_NEW_RETURN (connection_handler,
			Thr_Consumer_Handler (pci),
			0);

      // Create a reactive Consumer_Handler.
      else
#endif /* ACE_HAS_THREADS */
	ACE_NEW_RETURN (connection_handler,
			Consumer_Handler (pci),
			0);
    }
  else // connection_role == 'S', so configure a Supplier_Handler.
    {
#if defined (ACE_HAS_THREADS)
      // Create a threaded Supplier_Handler.
      if (ACE_BIT_ENABLED (Options::instance ()->threading_strategy (),
			   Options::INPUT_MT))
	ACE_NEW_RETURN (connection_handler,
			Thr_Supplier_Handler (pci),
			0);

      // Create a reactive Supplier_Handler.
      else
#endif /* ACE_HAS_THREAD */
	ACE_NEW_RETURN (connection_handler,
			Supplier_Handler (pci),
			0);
    }

  return connection_handler;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Map_Entry<Event_Key, Consumer_Dispatch_Set *>;
template class ACE_Map_Iterator_Base<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>;
template class ACE_Map_Iterator<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>;
template class ACE_Map_Reverse_Iterator<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>;
template class ACE_Map_Manager<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#if defined (ACE_HAS_THREADS)
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>;
#endif /* ACE_HAS_THREADS */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Map_Entry<Event_Key, Consumer_Dispatch_Set *>
#pragma instantiate ACE_Map_Iterator<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>
#pragma instantiate ACE_Map_Manager<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#if defined (ACE_HAS_THREADS)
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
#endif /* ACE_HAS_THREADS */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
