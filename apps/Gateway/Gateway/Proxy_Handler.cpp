// $Id$

#include "Event_Channel.h"
#include "Concrete_Proxy_Handlers.h"

void
Proxy_Handler::id (ACE_INT32 id)
{
  this->id_ = id;
}

ACE_INT32
Proxy_Handler::id (void)
{
  return this->id_;
}

// The total number of bytes sent/received on this Proxy.

size_t 
Proxy_Handler::total_bytes (void)
{
  return this->total_bytes_;
}

void 
Proxy_Handler::total_bytes (size_t bytes)
{
  this->total_bytes_ += bytes;
}

Proxy_Handler::Proxy_Handler (void)
{
}

Proxy_Handler::Proxy_Handler (const Proxy_Config_Info &pci)
  : remote_addr_ (pci.remote_port_, pci.host_),
    local_addr_ (pci.local_port_),
    id_ (pci.proxy_id_),
    total_bytes_ (0),
    state_ (Proxy_Handler::IDLE),
    timeout_ (1),
    max_timeout_ (pci.max_retry_timeout_),
    event_channel_ (pci.event_channel_)
{
  // Set the priority of the Proxy.
  this->priority (int (pci.priority_));
}

// Set the proxy_role.

void
Proxy_Handler::proxy_role (char d)
{
  this->proxy_role_ = d;
}

// Get the proxy_role.

char
Proxy_Handler::proxy_role (void)
{
  return this->proxy_role_;
}

// Sets the timeout delay.

void
Proxy_Handler::timeout (int to)
{
  if (to > this->max_timeout_)
    to = this->max_timeout_;

  this->timeout_ = to;
}

// Re-calculate the current retry timeout delay using exponential
// backoff.  Returns the original timeout (i.e., before the
// re-calculation).

int
Proxy_Handler::timeout (void)
{
  int old_timeout = this->timeout_;
  this->timeout_ *= 2;
  
  if (this->timeout_ > this->max_timeout_)
    this->timeout_ = this->max_timeout_;

  return old_timeout;
}

// Sets the max timeout delay.

void
Proxy_Handler::max_timeout (int mto)
{
  this->max_timeout_ = mto;
}

// Gets the max timeout delay.

int
Proxy_Handler::max_timeout (void)
{
  return this->max_timeout_;
}

// Restart connection asynchronously when timeout occurs.

int
Proxy_Handler::handle_timeout (const ACE_Time_Value &, 
			       const void *)
{
  ACE_DEBUG ((LM_DEBUG, 
	     "(%t) attempting to reconnect Proxy_Handler %d with timeout = %d\n", 
             this->id (), this->timeout_));

  // Delegate the re-connection attempt to the Event Channel.
  return this->event_channel_->initiate_proxy_connection 
    (this, ACE_Synch_Options::asynch);
}

// Handle shutdown of the Proxy_Handler object.

int
Proxy_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) shutting down %s Proxy_Handler %d on handle %d\n", 
	      this->proxy_role () == 'C' ? "Consumer" : "Supplier", 
	      this->id (), this->get_handle ()));
  
  // Restart the connection, if possible.
  return this->event_channel_->reinitiate_proxy_connection (this);
}

// Set the state of the Proxy.

void
Proxy_Handler::state (Proxy_Handler::State s)
{
  this->state_ = s;
}

// Upcall from the <ACE_Acceptor> or <ACE_Connector> that delegates
// control to our Proxy_Handler.

int
Proxy_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s Proxy_Handler's handle = %d\n",
	      this->proxy_role () == 'C' ? "Consumer" : "Supplier", 
	      this->peer ().get_handle ()));

  // Turn on non-blocking I/O.
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enable"), -1);

  // Call back to the <Event_Channel> to complete our initialization.
  else if (this->event_channel_->complete_proxy_connection (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "complete_proxy_connection"), -1);

  // Register ourselves to receive input events.
  else if (ACE_Service_Config::reactor ()->register_handler 
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);
  else
    return 0;
}

// Return the current state of the Proxy.

Proxy_Handler::State
Proxy_Handler::state (void)
{
  return this->state_;
}

ACE_INET_Addr &
Proxy_Handler::remote_addr (void)
{
  return this->remote_addr_;
}

ACE_INET_Addr &
Proxy_Handler::local_addr (void)
{
  return this->local_addr_;
}

// Make the appropriate type of <Proxy_Handler> (i.e.,
// <Consumer_Proxy>, <Supplier_Proxy>, <Thr_Consumer_Proxy>, or
// <Thr_Supplier_Proxy>).

Proxy_Handler *
Proxy_Handler_Factory::make_proxy_handler (const Proxy_Config_Info &pci)
{
  Proxy_Handler *proxy_handler = 0;

  // The next few lines of code are dependent on whether we are making
  // a threaded/reactive Supplier_Proxy/Consumer_Proxy.

  if (pci.proxy_role_ == 'C') // Configure a Consumer_Proxy.
    {
#if defined (ACE_HAS_THREADS)
      // Create a threaded Consumer_Proxy.
      if (ACE_BIT_ENABLED (pci.event_channel_->options ().threading_strategy_,
			   ACE_Event_Channel_Options::OUTPUT_MT))
	ACE_NEW_RETURN (proxy_handler,
			Thr_Consumer_Proxy (pci),
			0);

      // Create a reactive Consumer_Proxy.
      else
#endif /* ACE_HAS_THREADS */
	ACE_NEW_RETURN (proxy_handler,
			Consumer_Proxy (pci),
			0);
    }
  else // proxy_role == 'S', so configure a Supplier_Proxy.
    {
#if defined (ACE_HAS_THREADS)
      // Create a threaded Supplier_Proxy.
      if (ACE_BIT_ENABLED (pci.event_channel_->options ().threading_strategy_, 
			   ACE_Event_Channel_Options::INPUT_MT))
	ACE_NEW_RETURN (proxy_handler,
			Thr_Supplier_Proxy (pci),
			0);

      // Create a reactive Supplier_Proxy.
      else
#endif /* ACE_HAS_THREAD */
	ACE_NEW_RETURN (proxy_handler,
			Supplier_Proxy (pci),
			0);
    }

  return proxy_handler;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Map_Entry<Event_Key, Consumer_Dispatch_Set *>;
template class ACE_Map_Iterator<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>;
template class ACE_Map_Manager<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
