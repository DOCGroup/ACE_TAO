// CORBA_Handler.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Log_Msg.h"
#include "ace/CORBA_Handler.h"

#if !defined (__ACE_INLINE__)
#include "ace/CORBA_Handler.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_ORBIX)
ACE_ALLOC_HOOK_DEFINE(ACE_ST_CORBA_Handler)
ACE_ALLOC_HOOK_DEFINE(ACE_CORBA_Handler)

void
ACE_CORBA_Handler::dump (void) const
{
  ACE_TRACE ("ACE_CORBA_Handler::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "reactor_ = %x", this->reactor_));
  ACE_DEBUG ((LM_DEBUG, "\nreference_count_ = %d", this->reference_count_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_CORBA_Handler::ACE_CORBA_Handler (const ACE_CORBA_Handler &rhs)
{
  ACE_TRACE ("ACE_CORBA_Handler::ACE_CORBA_Handler");
}

const ACE_CORBA_Handler &
ACE_CORBA_Handler::operator= (const ACE_CORBA_Handler &rhs)
{
  ACE_TRACE ("ACE_CORBA_Handler::operator=");
  return *this;
}

void
ACE_ST_CORBA_Handler::dump (void) const
{
  ACE_TRACE ("ACE_ST_CORBA_Handler::dump");

  ACE_CORBA_Handler::dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "instance_ = %x", this->instance_));
  ACE_DEBUG ((LM_DEBUG, "\niteration_ = %d", this->iterations_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

ACE_CORBA_Handler::~ACE_CORBA_Handler (void)
{
  ACE_TRACE ("ACE_CORBA_Handler::~ACE_CORBA_Handler");
}

ACE_CORBA_Handler::ACE_CORBA_Handler (void)
  : reactor_ (ACE_Service_Config::reactor ())
{
  ACE_TRACE ("ACE_CORBA_Handler::ACE_CORBA_Handler");
}

// Only one ST CORBA Handler per-process...
/* static */ 
ACE_ST_CORBA_Handler *ACE_ST_CORBA_Handler::instance_ = 0;

// Insert a descriptor into the ACE_Reactor that Orbix has just added.

/* static */ 
void 
ACE_ST_CORBA_Handler::insert_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_ST_CORBA_Handler::insert_handle");
//  ACE_DEBUG ((LM_DEBUG, "+++ inserting %d\n", handle));

  if (ACE_ST_CORBA_Handler::instance_->reactor_ != 0)
    ACE_ST_CORBA_Handler::instance_->reactor_->register_handler 
      (handle, ACE_ST_CORBA_Handler::instance_, ACE_Event_Handler::READ_MASK);
  else
    ;
//    ACE_DEBUG ((LM_DEBUG, "insert_handle: reactor NULL\n"));
}

// Remove a descriptor from the ACE_Reactor that Orbix has just deleted.

/* static */ 
void 
ACE_ST_CORBA_Handler::remove_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_ST_CORBA_Handler::remove_handle");
//  ACE_DEBUG ((LM_DEBUG, "--- removing %d\n", handle));

  if (ACE_ST_CORBA_Handler::instance_->reactor_ != 0)
    ACE_ST_CORBA_Handler::instance_->reactor_->remove_handler
      (handle, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);
  else
    ;
//    ACE_DEBUG ((LM_DEBUG, "remove_handle: reactor NULL\n"));
}

// Process the next Orbix event.

int
ACE_ST_CORBA_Handler::handle_input (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_ST_CORBA_Handler::handle_input");
  //  ACE_DEBUG ((LM_DEBUG, "dispatching Orbix handle = %d in process
  //  = %P\n", handle));

  TRY {
    // Loop up to <ACE_ST_CORBA_Handler::iterations_> dispatching the
    // next event.  Note the trade off between efficiency and
    // fairness...
    
    for (size_t i = 0; i < this->iterations_; i++)
      if (ACE_CORBA_1 (Orbix.isEventPending) (IT_X))
        // Process the next Orbix event (don't block).
        ACE_CORBA_1 (Orbix.processNextEvent) (0, IT_X);
      else
        break;
  } 
  CATCHANY {
    // an error occured calling processNextEvent () - output the
    // error.
    cerr << IT_X << endl;
  } ENDTRY;
  return 0;
}

int
ACE_ST_CORBA_Handler::suspend (void)
{
  // Create an iterator.
  ACE_Handle_Set set (ACE_CORBA_1 (Orbix.getFileDescriptors) ());
  ACE_Handle_Set_Iterator orbix_descriptors (set);

  // Suspend all the HANDLEs registered by Orbix. 
  for (ACE_HANDLE h; 
       (h = orbix_descriptors ()) != ACE_INVALID_HANDLE; 
       ++orbix_descriptors)
    this->reactor_->suspend_handler (h);

  return 0;
}

int
ACE_ST_CORBA_Handler::resume (void)
{
  // Create an iterator.
  ACE_Handle_Set set (ACE_CORBA_1 (Orbix.getFileDescriptors) ());
  ACE_Handle_Set_Iterator orbix_descriptors (set);

  // Resume all the HANDLEs registered by Orbix. 
  for (ACE_HANDLE h; 
       (h = orbix_descriptors ()) != ACE_INVALID_HANDLE; 
       ++orbix_descriptors)
    this->reactor_->resume_handler (h);

  return 0;
}

// Dummy constructor.
ACE_ST_CORBA_Handler::ACE_ST_CORBA_Handler (void)
{
  ACE_TRACE ("ACE_ST_CORBA_Handler::ACE_ST_CORBA_Handler");

  // This is set by default for backward compatibility. The user can
  // use the set/get operations to change the iterations
  this->iterations_ = 5;

  // Set up the callbacks so that we get informed when Orbix changes
  // its descriptors.
  ACE_CORBA_1 (Orbix.registerIOCallback) ((OrbixIOCallback) &ACE_ST_CORBA_Handler::insert_handle, 
				   FD_OPEN_CALLBACK);
  ACE_CORBA_1 (Orbix.registerIOCallback) ((OrbixIOCallback) &ACE_ST_CORBA_Handler::remove_handle, 
				   FD_CLOSE_CALLBACK);
}      

void
ACE_ST_CORBA_Handler::get_orbix_descriptors (void)
{
  // Create an iterator.
  ACE_Handle_Set set (ACE_CORBA_1 (Orbix.getFileDescriptors) ());
  ACE_Handle_Set_Iterator orbix_descriptors (set);
  
  // Preinitialize anything that's already registered.
  for (ACE_HANDLE h; 
       (h = orbix_descriptors ()) != ACE_INVALID_HANDLE; 
       ++orbix_descriptors)
    ACE_ST_CORBA_Handler::insert_handle (h);
}

// Register <service_name> by doing a "putit" to register the
// <service_name> using the <marker_name> at <service_location> with
// orbixd.

/* static */
int
ACE_CORBA_Handler::register_service (const char *service_name,
				     const char *marker_name,
				     const char *service_location)
{
  ACE_TRACE ("ACE_CORBA_Handler::register_service");
  char buf[BUFSIZ * 2]; // I hope this is enough space...

  // Be defensive here...
  if (service_name == 0 || service_location == 0)
    {
      errno = EINVAL;
      return -1;
    }
  else if (marker_name == 0)
    ACE_OS::sprintf (buf, "putit %s %s", service_name, service_location);
  else
    ACE_OS::sprintf (buf, "putit -marker %s %s %s", 
	       marker_name, service_name, service_location);

  return ACE_OS::system (buf); // Use system(3S) to execute Orbix putit.
}

// Register <service_name> by doing a "putit" to register
// <service_name> using the <marker_name> with orbixd.

/* static */
int 
ACE_CORBA_Handler::remove_service (const char *service_name,
				   const char *marker_name)
{
  ACE_TRACE ("ACE_CORBA_Handler::remove_service");
  char buf[BUFSIZ * 2]; // I hope this is enough space!
  if (service_name == 0)
    {
      errno = EINVAL;
      return -1;
    }
  else if (marker_name == 0)
    ACE_OS::sprintf (buf, "rmit %s\n", service_name);
  else
    ACE_OS::sprintf (buf, "rmit -marker %s %s\n", marker_name, service_name);
  return ACE_OS::system (buf); // Use system(3S) to execute Orbix rmit.
}

ACE_ST_CORBA_Handler::~ACE_ST_CORBA_Handler (void)
{
  ACE_TRACE ("ACE_ST_CORBA_Handler::~ACE_ST_CORBA_Handler");
  // Create an iterator.
  ACE_Handle_Set set (ACE_CORBA_1 (Orbix.getFileDescriptors) ());
  ACE_Handle_Set_Iterator orbix_descriptors (set);
      
  // Remove everything!

  for (ACE_HANDLE h; 
       (h = orbix_descriptors ()) != ACE_INVALID_HANDLE; 
       ++orbix_descriptors)
    ACE_ST_CORBA_Handler::remove_handle (h);

  // Keep Orbix from calling us back and crashing the system!
  ACE_CORBA_1 (Orbix.registerIOCallback) (0, FD_OPEN_CALLBACK);
  ACE_CORBA_1 (Orbix.registerIOCallback) (0, FD_CLOSE_CALLBACK);
  ACE_ST_CORBA_Handler::instance_ = 0;
}

// Decrement the reference count and free up all the resources if this
// is the last service to be using the ACE_ST_CORBA_Handler...

/* static */
int 
ACE_CORBA_Handler::deactivate_service (const char *service_name,
				       const char *marker_name)
{
  ACE_TRACE ("ACE_CORBA_Handler::deactivate_service");
  if (service_name != 0
      && this->remove_service (service_name, marker_name) == -1)
    return -1;

  int ref_count = this->reference_count_;

  this->reference_count_--;

  // Close everything down if the count drops to 0.
  if (this->reference_count_ == 0)
    // Commit suicide!
    delete this;

  if (ref_count < 0)
    ;
//    ACE_DEBUG ((LM_DEBUG, "warning, reference count == %d\n",
//               ref_count));
  return 0;
}

/* static */ 
ACE_CORBA_Handler *
ACE_ST_CORBA_Handler::instance (void)
{
  ACE_TRACE ("ACE_ST_CORBA_Handler::instance");

  // Note that this does not need a double-check since it should be
  // called from a single-threaded environment.

  if (ACE_ST_CORBA_Handler::instance_ == 0)
    {
      ACE_NEW_RETURN (ACE_ST_CORBA_Handler::instance_, ACE_ST_CORBA_Handler, 0);
      ACE_ST_CORBA_Handler::instance_->get_orbix_descriptors ();
    }

  return ACE_ST_CORBA_Handler::instance_;
}

// Activate and register <service_name> with the Orbix daemon.  If
// <marker_name> and <service_location> are != 0 then do a "putit" to
// register this service with orbixd.  This method also increments the
// reference count of active services using the ACE_ST_CORBA_Handler.

int
ACE_CORBA_Handler::activate_service (const char *service_name, 
				     const char *marker_name, 
				     const char *service_location)
{
  ACE_TRACE ("ACE_CORBA_Handler::activate_service");
  // Since the ACE_CORBA_Handler is a singleton, make sure not to
  // allocate and initialize more than one copy.  By incrementing the
  // reference count we ensure this.

  this->reference_count_++;

  if (service_name != 0 && service_location != 0
      && this->register_service (service_name, marker_name, 
				 service_location) == -1)
    return -1;

  // Tell Orbix that we have completed the server's initialization.
  // Note that we don't block by giving a timeout of 0...

  TRY {
    ACE_CORBA_1 (Orbix.impl_is_ready) ((char *) service_name, 0, IT_X);
  } CATCHANY {
    return -1;
  } ENDTRY

  return 0;
}

#if defined (ACE_HAS_MT_ORBIX)

ACE_ALLOC_HOOK_DEFINE(ACE_MT_CORBA_Handler)

#if defined (ACE_MT_SAFE)
// Synchronize output operations.
ACE_Thread_Mutex ACE_MT_CORBA_Handler::ace_mt_corba_handler_lock_;
#endif /* ACE_MT_SAFE */

void
ACE_MT_CORBA_Handler::dump (void) const
{
  ACE_TRACE ("ACE_MT_CORBA_Handler::dump");
  ACE_CORBA_Handler::dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, "instance_ = %x", this->instance_));
  ACE_DEBUG ((LM_DEBUG, "\nthr_mgr_ = %x", this->thr_mgr_));
  this->pipe_.dump ();
#if defined (ACE_MT_SAFE)
  // Double-Check lock.
  ace_mt_corba_handler_lock_.dump ();
#endif /* ACE_MT_SAFE */
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Only one MT CORBA Handler per-process...
/* static */ 
ACE_MT_CORBA_Handler *ACE_MT_CORBA_Handler::instance_ = 0;

/* static */
ACE_CORBA_Handler *
ACE_MT_CORBA_Handler::instance (void)
{
  ACE_TRACE ("ACE_MT_CORBA_Handler::instance");

  if (ACE_MT_CORBA_Handler::instance_ == 0)
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ACE_MT_CORBA_Handler::ace_mt_corba_handler_lock_, 0);

      if (ACE_MT_CORBA_Handler::instance_ == 0)
	ACE_NEW_RETURN (ACE_MT_CORBA_Handler::instance_,
			ACE_MT_CORBA_Handler, 0);
    }

  return ACE_MT_CORBA_Handler::instance_;
}

int
ACE_MT_CORBA_Handler::suspend (void)
{
  // Suspend the event handler listening for new CORBA requests to
  // dispatch.
  this->reactor_->suspend_handler (this->pipe_.read_handle ());

  // Suspend the daemon thread.
  this->thr_mgr ()->suspend_all ();
  return 0;
}

int
ACE_MT_CORBA_Handler::resume (void)
{
  // Resume the event handler listening for new CORBA requests to
  // dispatch.
  this->reactor_->resume_handler (this->pipe_.read_handle ());

  // Resume the daemon thread.
  this->thr_mgr ()->resume_all ();
  return 0;
}

ACE_MT_CORBA_Handler::ACE_MT_CORBA_Handler (void)
{
  ACE_TRACE ("ACE_MT_CORBA_Handler::ACE_MT_CORBA_Handler");
  this->thr_mgr (ACE_Service_Config::thr_mgr ());

  int result = 0;

  if (this->pipe_.open () == -1)
    result = -1;
  // Register one end of the pipe with the reactor with a READ mask.
  else if (this->reactor_->register_handler
      (this->pipe_.read_handle (), this, ACE_Event_Handler::READ_MASK) == -1) 
    result = -1;
  // Create a new thread that processes events for the Orbix event
  // queue.
  else if (this->thr_mgr ()->spawn (ACE_THR_FUNC (ACE_MT_CORBA_Handler::process_events),
				    0, THR_DETACHED | THR_NEW_LWP) == -1)
    result = -1;
  
  if (result == -1)
    {
      delete ACE_MT_CORBA_Handler::instance_;
      ACE_MT_CORBA_Handler::instance_ = 0;
    }
}

void *
ACE_MT_CORBA_Handler::process_events (void *)
{
  ACE_TRACE ("ACE_MT_CORBA_Handler::process_events");

  // Special knowlege, we "know" that we are dealing with a singleton
  // and that we are invoked in a context where the mutex controlling
  // instance creation is held, so by the time we get the mutex
  // the instance must exist.
  if (ACE_MT_CORBA_Handler::instance_ == 0)
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, 
		        ace_mon, 
		        ace_mt_corba_handler_lock_,
			0);
      ACE_ASSERT (ACE_MT_CORBA_Handler::instance_ != 0);
    }

  ACE_Thread_Control t (ACE_MT_CORBA_Handler::instance_->thr_mgr ());
  
  // This thread only processes events.
  TRY {
    // it is OK to block
    ACE_CORBA_1 (Orbix.processEvents) (ACE_CORBA_1 (Orbix.INFINITE_TIMEOUT), IT_X);
  } CATCHANY {
    // An error occured calling processEvents () - output the error.
    cerr << IT_X << endl;
  } ENDTRY;

  // Thread dies if we reach here : error occured in processEvents.
  return 0;
}

int
ACE_MT_CORBA_Handler::inRequestPreMarshal (ACE_CORBA_1 (Request) &req,
					   ACE_CORBA_1 (Environment) &IT_env)
{
  ACE_TRACE ("ACE_MT_CORBA_Handler::inRequestPreMarshal");

  // Pump the request through the pipe.
  u_long request_addr = (u_long) &req;

  ssize_t result = ACE::send (this->pipe_.write_handle (),
			      (const char *) &request_addr, 
			      sizeof request_addr);

  if (result != sizeof request_addr)
    {
      // Don't continue with request
      return 0;
    }

  // Everything is fine: we have delegated the work to a different
  // thread Tell Orbix we will dispatch the request later...
  return -1;
}

int
ACE_MT_CORBA_Handler::handle_input (ACE_HANDLE)
{
  ACE_TRACE ("ACE_MT_CORBA_Handler::handle_input");
  ACE_CORBA_1 (Request) *req = 0;

  u_long request_addr;

  //  Read the request from the pipe.
  ssize_t result = ACE::recv (this->pipe_.read_handle (),
			      (char *) &request_addr, 
			      sizeof request_addr);

  if (result != sizeof request_addr)
    // We are in trouble: bail out.
    return -1;

  req = (ACE_CORBA_1 (Request) *) request_addr;

  // Tell Orbix to dispatch the request.
  ACE_CORBA_1 (Orbix.continueThreadDispatch) (*req);
  return 0;
}

ACE_MT_CORBA_Handler::~ACE_MT_CORBA_Handler (void)
{
  ACE_TRACE ("ACE_MT_CORBA_Handler::~ACE_MT_CORBA_Handler");

  // Unregister one end of the pipe with the reactor 
  this->reactor_->remove_handler 
    (this->pipe_.read_handle (),
     ACE_Event_Handler::READ_MASK | ACE_Event_Handler::DONT_CALL);

  this->pipe_.close ();

  ACE_MT_CORBA_Handler::instance_ = 0;
}
#endif /* ACE_HAS_MT_ORBIX */
#endif /* ACE_HAS_ORBIX */
