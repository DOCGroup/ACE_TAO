// Acceptor.cpp
// $Id$

#if !defined (ACE_ACCEPTOR_C)
#define ACE_ACCEPTOR_C

#define ACE_BUILD_DLL
#include "ace/ACE.h"
#include "ace/Acceptor.h"

ACE_ALLOC_HOOK_DEFINE(ACE_Acceptor)

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> void
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::dump (void) const
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->peer_acceptor_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::operator ACE_PEER_ACCEPTOR & () const
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::operator ACE_PEER_ACCEPTOR &");
  return (ACE_PEER_ACCEPTOR &) this->peer_acceptor_;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_PEER_ACCEPTOR &
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor (void) const
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor");
  return (ACE_PEER_ACCEPTOR &) this->peer_acceptor_;
}

// Returns ACE_HANDLE of the underlying Acceptor_Strategy.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_HANDLE
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle (void) const
{ 
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle");
  return this->peer_acceptor_.get_handle (); 
}

// Initialize the appropriate strategies for creation, passive
// connection acceptance, and concurrency, and then register <this>
// with the Reactor and listen for connection requests at the
// designated <local_addr>.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open 
  (const ACE_PEER_ACCEPTOR_ADDR &local_addr, 
   ACE_Reactor *reactor)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open");
  this->reactor (reactor);

  // Must supply a valid Reactor to Acceptor::open()...

  if (reactor == 0)
    {
      errno = EINVAL;
      return -1;
    }

  if (this->peer_acceptor_.open (local_addr, 1) == -1)
    return -1;

  return this->reactor ()->register_handler 
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

// Simple constructor.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Acceptor (ACE_Reactor *reactor)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Acceptor");

  this->reactor (reactor);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Acceptor 
  (const ACE_PEER_ACCEPTOR_ADDR &addr, 
   ACE_Reactor *reactor)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Acceptor");
  if (this->open (addr, reactor) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Acceptor::ACE_Acceptor"));
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Acceptor (void)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Acceptor");
  this->handle_close ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::fini (void)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::fini");
  return ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_close ();
}

// Hook called by the explicit dynamic linking facility.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::init (int, char *[])
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::init");
  return -1;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::info (char **strp, 
						      size_t length) const
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::info");
  char buf[BUFSIZ];
  char addr_str[BUFSIZ];
  ACE_PEER_ACCEPTOR_ADDR addr;

  if (this->acceptor ().get_local_addr (addr) == -1)
    return -1;
  else if (addr.addr_to_string (addr_str, sizeof addr) == -1)
    return -1;

  ACE_OS::sprintf (buf, "%s\t %s %s", 
		   "ACE_Acceptor", addr_str, "# acceptor factory\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::suspend (void)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::suspend");
  return this->reactor ()->suspend_handler (this);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::resume (void)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::resume");
  return this->reactor ()->resume_handler (this);
}

// Perform termination activities when <this> is removed from the
// <reactor>.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_close (ACE_HANDLE,
							      ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_close");
  // Guard against multiple closes.
  if (this->reactor () != 0)  
    {
      ACE_HANDLE handle = this->get_handle ();

      // We must use the <handle> obtained *before* we deleted the
      // accept_strategy_...

      this->reactor_->remove_handler 
	(handle, ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL);

      // Shut down the listen socket to recycle the handles.
      if (this->peer_acceptor_.close () == -1)
	ACE_ERROR ((LM_ERROR, "close\n"));

      // Set the Reactor to 0 so that we don't try to close down
      // again.
      this->reactor (0);
    }
  return 0;
}

// Bridge method for creating a SVC_HANDLER.  The strategy for
// creating a SVC_HANDLER are configured into the Acceptor via it's
// <creation_strategy_>.  The default is to create a new SVC_HANDLER.
// However, subclasses can override this strategy to perform
// SVC_HANDLER creation in any way that they like (such as creating
// subclass instances of SVC_HANDLER, using a singleton, dynamically
// linking the handler, etc.).

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::make_svc_handler (SVC_HANDLER *&sh)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::make_svc_handler");
  if (sh == 0)
    ACE_NEW_RETURN (sh, SVC_HANDLER, -1);
  return 0;
}

// Bridge method for accepting the new connection into the
// <svc_handler>.  The default behavior delegates to the
// PEER_ACCEPTOR::accept() in the Acceptor_Strategy.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler 
  (SVC_HANDLER *svc_handler)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler");
  if (this->peer_acceptor_.accept (svc_handler->peer ()) == -1)
    {
      // Close down handler to avoid memory leaks.
      svc_handler->close (0); 
      return -1;
    }
  else
    return 0;
}

// Bridge method for activating a <svc_handler> with the appropriate
// concurrency strategy.  The default behavior of this method is to
// activate the SVC_HANDLER by calling its open() method (which allows
// the SVC_HANDLER to define its own concurrency strategy).  However,
// subclasses can override this strategy to do more sophisticated
// concurrency activations (such as creating the SVC_HANDLER as an
// "active object" via multi-threading or multi-processing).

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::activate_svc_handler 
  (SVC_HANDLER *svc_handler)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::activate_svc_handler");
  if (svc_handler->open ((void *) this) == -1)
    {
      svc_handler->close (0);
      return -1;
    }
  else
    return 0;
}

// Template Method that makes a SVC_HANDLER (using the appropriate
// creation strategy), accept the connection into the SVC_HANDLER, and
// then activate the SVC_HANDLER.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_input (ACE_HANDLE listener)
{
  ACE_TRACE ("ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_input");
  ACE_Handle_Set conn_handle;

  // Default is "timeout (0, 0)," which means "poll."
  ACE_Time_Value timeout; 
  
  // Accept connections from clients (note that a loop is used for two
  // reasons:
  // 
  // 1. It allows us to accept all pending connections without an
  //    extra trip through the ACE_Reactor and without having to use
  //    non-blocking I/O...
  // 
  // 2. It allows the TLI_SAP::ACE_Acceptor class to work correctly (don't
  //    ask -- TLI is *horrible*...)).

  // @@ What should we do if any of the substrategies fail?  Right
  // now, we just log an error message and return 0 (which means that
  // the Acceptor remains registered with the Reactor)...
  do
    {
      // Create a service handler, using the appropriate creation
      // strategy.

      SVC_HANDLER *svc_handler = 0;
      
      if (this->make_svc_handler (svc_handler) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "make_svc_handler"), 0);

      // Accept connection into the Svc_Handler.

      else if (this->accept_svc_handler (svc_handler) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "accept_svc_handler"), 0);

      // Activate the <svc_handler> using the designated concurrency
      // strategy (note that this method becomes responsible for
      // handling errors and freeing up the memory if things go
      // awry...).

      else if (this->activate_svc_handler (svc_handler) == -1)
      	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "activate_svc_handler"), 0);

      conn_handle.set_bit (listener);
    }
  // Now, check to see if there is another connection pending and
  // break out of the loop if there is none.
  while (ACE_OS::select (int (listener) + 1, conn_handle, 0, 0, &timeout) == 1);
  return 0;
}

ACE_ALLOC_HOOK_DEFINE(ACE_Strategy_Acceptor)

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::suspend (void)
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::suspend");

  // First suspend the SVC_HANDLER's we've created.
  if (this->scheduling_strategy_->suspend () == -1)
    return -1;
  else   // Then suspend ourselves.
    return ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::suspend ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::resume (void)
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::resume");

  // First resume ourselves.
  if (ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::suspend () == -1)
    return -1;
  else // Then resume the SVC_HANDLER's we've created.
    return this->scheduling_strategy_->resume ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> void
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::dump (void) const
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::dump ();
  this->creation_strategy_->dump ();
  ACE_DEBUG ((LM_DEBUG, "delete_creation_strategy_ = %d", delete_creation_strategy_));
  this->accept_strategy_->dump ();
  ACE_DEBUG ((LM_DEBUG, "delete_accept_strategy_ = %d", delete_accept_strategy_));
  this->concurrency_strategy_->dump ();
  ACE_DEBUG ((LM_DEBUG, "delete_concurrency_strategy_ = %d", delete_concurrency_strategy_));
  this->scheduling_strategy_->dump ();
  ACE_DEBUG ((LM_DEBUG, "delete_scheduling_strategy_ = %d", delete_scheduling_strategy_));
  ACE_DEBUG ((LM_DEBUG, "\nservice_name_ = %s", this->service_name_));
  ACE_DEBUG ((LM_DEBUG, "\nservice_description_ = %s", this->service_description_));
  ACE_DEBUG ((LM_DEBUG, "\nservice_port_ = %d", this->service_port_));
  this->service_addr_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_PEER_ACCEPTOR &
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor (void) const
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor");
  return this->accept_strategy_->acceptor ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::operator ACE_PEER_ACCEPTOR & () const
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::operator ACE_PEER_ACCEPTOR &");
  return this->accept_strategy_->acceptor ();
}

// Returns ACE_HANDLE of the underlying Acceptor_Strategy.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_HANDLE
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle (void) const
{ 
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle");
  return this->accept_strategy_->get_handle (); 
}

// Initialize the appropriate strategies for creation, passive
// connection acceptance, and concurrency, and then register <this>
// with the Reactor and listen for connection requests at the
// designated <local_addr>.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open 
  (const ACE_PEER_ACCEPTOR_ADDR &local_addr, 
   ACE_Reactor *reactor,
   ACE_Creation_Strategy<SVC_HANDLER> *cre_s,
   ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> *acc_s,
   ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
   ACE_Scheduling_Strategy<SVC_HANDLER> *sch_s,
   const char service_name[],
   const char service_description[])
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open");

  if (this->service_name_ == 0 && service_name != 0)
    this->service_name_ = ACE_OS::strdup (service_name);
  if (this->service_description_ == 0 && service_description != 0)
    this->service_description_ = ACE_OS::strdup (service_description);

  this->reactor (reactor);

  // Must supply a valid Reactor to Acceptor::open()...
  if (reactor == 0)
    {
      errno = EINVAL;
      return -1;
    }

  // Initialize the creation strategy.

  if (cre_s == 0)
    {
      ACE_NEW_RETURN (cre_s, CREATION_STRATEGY, -1);
      this->delete_creation_strategy_ = 1;
    }
  this->creation_strategy_ = cre_s;

  // Initialize the accept strategy.

  if (acc_s == 0)
    {
      ACE_NEW_RETURN (acc_s, ACCEPT_STRATEGY, -1);
      this->delete_accept_strategy_ = 1;
    }
  this->accept_strategy_ = acc_s;

  if (this->accept_strategy_->open (local_addr, 1) == -1)
    return -1;

  // Initialize the concurrency strategy.

  if (con_s == 0)
    {
      ACE_NEW_RETURN (con_s, CONCURRENCY_STRATEGY, -1);
      this->delete_concurrency_strategy_ = 1;
    }
  this->concurrency_strategy_ = con_s;

  // Initialize the scheduling strategy.

  if (sch_s == 0)
    {
      ACE_NEW_RETURN (sch_s, SCHEDULING_STRATEGY, -1);
      this->delete_scheduling_strategy_ = 1;
    }
  this->scheduling_strategy_ = sch_s;

  return this->reactor ()->register_handler 
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

// Simple constructor.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Strategy_Acceptor 
  (const char service_name[],
   const char service_description[])
    : creation_strategy_ (0),
      delete_creation_strategy_ (0),
      accept_strategy_ (0),
      delete_accept_strategy_ (0),
      concurrency_strategy_ (0),
      delete_concurrency_strategy_ (0),
      scheduling_strategy_ (0),
      delete_scheduling_strategy_ (0),
      service_name_ (0),
      service_description_ (0)
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Strategy_Acceptor");
  
  if (service_name != 0)
    this->service_name_ = ACE_OS::strdup (service_name);
  if (service_description != 0)
    this->service_description_ = ACE_OS::strdup (service_description);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Strategy_Acceptor 
  (const ACE_PEER_ACCEPTOR_ADDR &addr, 
   ACE_Reactor *reactor,
   ACE_Creation_Strategy<SVC_HANDLER> *cre_s,
   ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> *acc_s,
   ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
   ACE_Scheduling_Strategy<SVC_HANDLER> *sch_s,
   const char service_name[],
   const char service_description[])
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Strategy_Acceptor");

  if (this->open (addr, reactor, cre_s, acc_s, con_s, sch_s,
		  service_name, service_description) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "ACE_Strategy_Acceptor::ACE_Strategy_Acceptor"));
}

// Perform termination activities when <this> is removed from the
// <ACE_Reactor>.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_close (ACE_HANDLE,
								       ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_close");
  // Guard against multiple closes.
  if (this->reactor () != 0)  
    {
      ACE_HANDLE handle = this->get_handle ();

      if (this->delete_creation_strategy_)
	delete this->creation_strategy_;
      this->delete_creation_strategy_ = 0;
      this->creation_strategy_ = 0;

      if (this->delete_accept_strategy_)
	delete this->accept_strategy_;
      this->delete_accept_strategy_ = 0;
      this->accept_strategy_ = 0;

      if (this->delete_concurrency_strategy_)
	delete this->concurrency_strategy_;
      this->delete_concurrency_strategy_ = 0;
      this->concurrency_strategy_ = 0;

      if (this->delete_scheduling_strategy_)
	delete this->scheduling_strategy_;
      this->delete_scheduling_strategy_ = 0;
      this->scheduling_strategy_ = 0;

      // We must use the <handle> obtained *before* we deleted the
      // accept_strategy_...

      this->reactor ()->remove_handler 
	(handle, ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL);

      // Set the Reactor to 0 so that we don't try to close down
      // again.
      this->reactor (0);
    }
  return 0;
}

// Bridge method for creating a SVC_HANDLER.  The strategy for
// creating a SVC_HANDLER are configured into the Acceptor via it's
// <creation_strategy_>.  The default is to create a new SVC_HANDLER.
// However, subclasses can override this strategy to perform
// SVC_HANDLER creation in any way that they like (such as creating
// subclass instances of SVC_HANDLER, using a singleton, dynamically
// linking the handler, etc.).

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::make_svc_handler (SVC_HANDLER *&sh)
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::make_svc_handler");
  return this->creation_strategy_->make_svc_handler (sh);
}

// Bridge method for accepting the new connection into the
// <svc_handler>.  The default behavior delegates to the
// <Strategy_Acceptor::accept> in the Acceptor_Strategy.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler 
  (SVC_HANDLER *svc_handler)
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler");
  return this->accept_strategy_->accept_svc_handler (svc_handler);
}

// Bridge method for activating a <svc_handler> with the appropriate
// concurrency strategy.  The default behavior of this method is to
// activate the SVC_HANDLER by calling its open() method (which allows
// the SVC_HANDLER to define its own concurrency strategy).  However,
// subclasses can override this strategy to do more sophisticated
// concurrency activations (such as creating the SVC_HANDLER as an
// "active object" via multi-threading or multi-processing).

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::activate_svc_handler 
  (SVC_HANDLER *svc_handler)
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::activate_svc_handler");
  return this->concurrency_strategy_->activate_svc_handler 
    (svc_handler, (void *) this);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Strategy_Acceptor (void)
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Strategy_Acceptor");
  ACE_OS::free ((void *) this->service_name_);
  ACE_OS::free ((void *) this->service_description_);
  this->handle_close ();
}

// Signal the server to shutdown gracefully.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int 
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_signal (int, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_Strategy_Acceptor::handle_signal");
  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::info (char **strp, 
					  size_t length) const
{
  ACE_TRACE ("ACE_Strategy_Acceptor::info");
  char buf[BUFSIZ];
  char service_addr_str[BUFSIZ];
  ACE_PEER_ACCEPTOR_ADDR addr;

  if (this->acceptor ().get_local_addr (addr) == -1)
    return -1;
  else if (addr.addr_to_string (service_addr_str, sizeof addr) == -1)
    return -1;

  // @@ Should add the protocol in...
  ACE_OS::sprintf (buf, "%s\t %s #%s\n",
		   this->service_name_, 
		   service_addr_str, 
		   this->service_description_);

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::fini (void)
{
  ACE_TRACE ("ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::fini");
  return this->ACE_Strategy_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_close ();
}

ACE_ALLOC_HOOK_DEFINE(ACE_Oneshot_Acceptor)

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> void
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::dump (void) const
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "\nsvc_handler_ = %x", this->svc_handler_));
  ACE_DEBUG ((LM_DEBUG, "\nrestart_ = %d", this->restart_));
  this->peer_acceptor_.dump ();
  ACE_DEBUG ((LM_DEBUG, "delete_concurrency_strategy_ = %d", 
	      delete_concurrency_strategy_));
  this->concurrency_strategy_->dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open 
  (const ACE_PEER_ACCEPTOR_ADDR &addr,
   ACE_Reactor *reactor,
   ACE_Concurrency_Strategy<SVC_HANDLER> *con_s)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open");
  this->reactor (reactor);

  // Initialize the concurrency strategy.

  if (con_s == 0)
    {
      ACE_NEW_RETURN (con_s, ACE_Concurrency_Strategy<SVC_HANDLER>, -1);
      this->delete_concurrency_strategy_ = 1;
    }
  this->concurrency_strategy_ = con_s;

  // Reuse the addr, even if it is already in use...!
  return this->peer_acceptor_.open (addr, 1);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Oneshot_Acceptor (void)
  : delete_concurrency_strategy_ (0)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Oneshot_Acceptor");
  this->reactor (0);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Oneshot_Acceptor 
  (const ACE_PEER_ACCEPTOR_ADDR &addr,
   ACE_Reactor *reactor,
   ACE_Concurrency_Strategy<SVC_HANDLER> *cs)
    : delete_concurrency_strategy_ (0)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::ACE_Oneshot_Acceptor");
  if (this->open (addr, reactor, cs) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", 
	       "ACE_Oneshot_Acceptor::ACE_Oneshot_Acceptor"));
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Oneshot_Acceptor (void)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::~ACE_Oneshot_Acceptor");
  this->handle_close ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_close (ACE_HANDLE,
								      ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_close");
  // Guard against multiple closes.
  if (this->concurrency_strategy_ != 0)  
    {
      if (this->delete_concurrency_strategy_)
	delete this->concurrency_strategy_;
      this->delete_concurrency_strategy_ = 0;
      this->concurrency_strategy_ = 0;

      // Note that if we aren't actually registered with the
      // ACE_Reactor then it's ok for this call to fail...

      if (this->reactor ())
	this->reactor ()->remove_handler 
	  (this, ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL);

      if (this->peer_acceptor_.close () == -1)
	ACE_ERROR ((LM_ERROR, "close\n"));
    }
  return 0;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_timeout 
  (const ACE_Time_Value &tv, 
   const void *arg)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_timeout");
  errno = ETIMEDOUT;

  if (this->svc_handler_->handle_timeout (tv, arg) == -1)
    this->svc_handler_->handle_close (sh->get_handle (), 
				      ACE_Event_Handler::TIMER_MASK);;

  // Since we aren't necessarily registered with the Reactor, don't
  // bother to check the return value here...
  if (this->reactor ())
	this->reactor ()->remove_handler (this, ACE_Event_Handler::ACCEPT_MASK);
  return 0;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::cancel (void)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::cancel");
  return this->reactor () && this->reactor ()->cancel_timer (this);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::register_handler 
  (SVC_HANDLER *svc_handler,
   const ACE_Synch_Options &synch_options,
   int restart)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::register_handler");
  // Can't do this if we don't have a Reactor.
  if (this->reactor () == 0)
    return -1;
  else
    {
      this->svc_handler_ = svc_handler;
      this->restart_ = restart;
      ACE_Time_Value *tv = (ACE_Time_Value *) synch_options.time_value ();

      if (tv != 0 
	  && this->reactor ()->schedule_timer (this, synch_options.arg (),
					     *tv) == 0)
	return -1;
      else
	return this->reactor ()->register_handler 
	  (this, ACE_Event_Handler::ACCEPT_MASK);
    }
}

// Bridge method for activating a <svc_handler> with the appropriate
// concurrency strategy.  The default behavior of this method is to
// activate the SVC_HANDLER by calling its open() method (which allows
// the SVC_HANDLER to define its own concurrency strategy).  However,
// subclasses can override this strategy to do more sophisticated
// concurrency activations (such as creating the SVC_HANDLER as an
// "active object" via multi-threading or multi-processing).

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::activate_svc_handler 
  (SVC_HANDLER *svc_handler)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::activate_svc_handler");
  return this->concurrency_strategy_->activate_svc_handler 
    (svc_handler, (void *) this);
}

// Factors out the code shared between the <accept> and <handle_input>
// methods.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::shared_accept 
  (SVC_HANDLER *svc_handler,
   ACE_PEER_ACCEPTOR_ADDR *remote_addr,
   ACE_Time_Value *timeout,
   int restart)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::shared_accept");
  if (svc_handler == 0)
    return -1;

  // Accept connection into the Svc_Handler.
  else if (this->peer_acceptor_.accept (svc_handler->peer (), remote_addr,
					timeout, restart) == -1)
    {
      // Check whether we just timed out or whether we failed...
      if (!(errno == EWOULDBLOCK || errno == ETIMEDOUT))
	// Close down handler to avoid memory leaks.
	svc_handler->close (0); 
      return -1;
    }
  // Activate the <svc_handler> using the designated concurrency
  // strategy (note that this method becomes responsible for
  // handling errors and freeing up the memory if things go
  // awry...)
  else
    return this->activate_svc_handler (svc_handler);
}

// Make a SVC_HANDLER, accept the connection into the SVC_HANDLER, and
// then activate the SVC_HANDLER.  Note that SVC_HANDLER::open()
// decides what type of concurrency strategy to use.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept 
  (SVC_HANDLER *svc_handler,
   ACE_PEER_ACCEPTOR_ADDR *remote_addr,
   const ACE_Synch_Options &synch_options,
   int restart)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept");
  // Note that if timeout == ACE_Time_Value (x, y) where (x > 0 || y >
  // 0) then this->connector_.connect() will block synchronously.  If
  // <use_reactor> is set then we don't want this to happen (since we
  // want the ACE_Reactor to do the timeout asynchronously).
  // Therefore, we'll force this->connector_ to use ACE_Time_Value (0,
  // 0) in this case...

  ACE_Time_Value *timeout;
  int use_reactor = synch_options[ACE_Synch_Options::USE_REACTOR];

  if (use_reactor)
    timeout = (ACE_Time_Value *) &ACE_Time_Value::zero;
  else
    timeout = (ACE_Time_Value *) synch_options.time_value ();

  if (this->shared_accept (svc_handler, remote_addr, timeout, restart) == -1)
    {
      if (use_reactor && errno == EWOULDBLOCK)
	// We couldn't accept right away, so let's wait in the ACE_Reactor.
	this->register_handler (svc_handler, synch_options, restart);
      return -1;
    }
  return 0;
}

// Accepts one pending connection from a client (since we're the
// "oneshot" Acceptor).

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_input (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::handle_input");
  int result = 0;

  // Cancel any timer that might be pending.
  this->cancel ();

  if (this->shared_accept (this->svc_handler_, 0, 0, this->restart_) == -1)
    result = -1;
  if (this->reactor () && this->reactor ()->remove_handler 
      (this, ACE_Event_Handler::ACCEPT_MASK | ACE_Event_Handler::DONT_CALL) == -1)
    result = -1;
  return result;
}

// Hook called by the explicit dynamic linking facility.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::init (int, char *[])
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::init");
  return -1;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::fini (void)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::fini");
  return this->handle_close ();
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::info (char **strp, 
							      size_t length) const
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::info");
  char buf[BUFSIZ];
  char addr_str[BUFSIZ];
  ACE_PEER_ACCEPTOR_ADDR addr;

  if (this->peer_acceptor_.get_local_addr (addr) == -1)
    return -1;
  else if (addr.addr_to_string (addr_str, sizeof addr) == -1)
    return -1;

  ACE_OS::sprintf (buf, "%s\t %s %s", "ACE_Oneshot_Acceptor", 
	     addr_str, "#oneshot acceptor factory\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::suspend (void)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::suspend");
  return this->reactor () && this->reactor ()->suspend_handler (this);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::resume (void)
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::resume");
  return this->reactor () && this->reactor ()->resume_handler (this);
}

// Returns ACE_HANDLE of the underlying peer_acceptor.

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_HANDLE
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle (void) const
{ 
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::get_handle");
  return this->peer_acceptor_.get_handle (); 
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> ACE_PEER_ACCEPTOR &
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor (void) const
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::acceptor");
  return (ACE_PEER_ACCEPTOR &) this->peer_acceptor_;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> 
ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::operator ACE_PEER_ACCEPTOR & () const
{
  ACE_TRACE ("ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::operator ACE_PEER_ACCEPTOR &");
  return (ACE_PEER_ACCEPTOR &) this->peer_acceptor_;
}

#endif /* ACE_ACCEPTOR_C */
