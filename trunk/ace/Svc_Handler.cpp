// Svc_Handler.cpp
// $Id$

#if !defined (ACE_SVC_HANDLER_C)
#define ACE_SVC_HANDLER_C

#define ACE_BUILD_DLL
#include "ace/Svc_Handler.h"
#include "ace/Dynamic.h"

#if !defined (__ACE_INLINE__)
#include "ace/Svc_Handler.i"
#endif /* __ACE_INLINE__ */

#define PR_ST_1 ACE_PEER_STREAM_1
#define PR_ST_2 ACE_PEER_STREAM_2

#if defined (ACE_MT_SAFE) && !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) 
// Lock the creation of the Singleton.
template <PR_ST_1, ACE_SYNCH_1>
ACE_Thread_Mutex ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::ace_svc_handler_lock_;
#endif /* defined (ACE_MT_SAFE) && !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

template <PR_ST_1, ACE_SYNCH_1> ACE_Dynamic *
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::instance (void)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::allocated");

#if defined (ACE_MT_SAFE) && defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) 
  // Lock the creation of the Singleton.  This should be inside of
  // ACE_Svc_Handler, but GNU G++ is too lame to handle this...
  static ACE_Thread_Mutex ace_svc_handler_lock_;
#endif /* defined (ACE_MT_SAFE) && defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */

  static ACE_TSS_TYPE (ACE_Dynamic) *instance_;
  // Determines if we were dynamically allocated.  Note that this
  // should be inside of ACE_Svc_Handler, but G++ is too lame to
  // support this...

  // Implement the Double Check pattern.

  if (instance_ == 0)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, ace_svc_handler_lock_, 0));

      if (instance_ == 0)
	ACE_NEW_RETURN (instance_, ACE_TSS_TYPE (ACE_Dynamic), 0);
    }

  return ACE_TSS_GET (instance_, ACE_Dynamic);
}

template <PR_ST_1, ACE_SYNCH_1> void *
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::operator new (size_t n)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::operator new");
  // Allocate the memory and store it (usually in thread-specific
  // storage, depending on config flags).
  return ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_SYNCH_2>::instance ()->set (::new char[n]);
}

template <PR_ST_1, ACE_SYNCH_1> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::destroy (void) 
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::destroy");

  // Only delete ourselves if we've been allocated dynamically.
  if (this->dynamic_)
    // Will call the destructor, which automatically calls <shutdown>.
    // Note that if we are *not* allocated dynamically then the
    // destructor will call <shutdown> automatically when it gets run
    // during cleanup.
    delete this; 
}

template <PR_ST_1, ACE_SYNCH_1> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::operator delete (void *obj) 
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::delete");
  ::delete obj;
}

/* Default constructor */

template <PR_ST_1, ACE_SYNCH_1> 
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::ACE_Svc_Handler (ACE_Thread_Manager *tm,
							ACE_Message_Queue<ACE_SYNCH_2> *mq,
							ACE_Reactor *reactor)
  : ACE_Task<ACE_SYNCH_2> (tm, mq)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::ACE_Svc_Handler");
  
  this->reactor (reactor);

  // This clever idiom transparently checks if we were allocated
  // dynamically.  This information is used by the <destroy> method to
  // decide if we need to delete <this>...  The idiom is based on a
  // paper by Michael van Rooyen (mrooyen@cellnet.co.uk) that appeared
  // in the April '96 issue of the C++ Report.  We've spruced it up to
  // work correctly in multi-threaded programs by using our ACE_TSS
  // class.
  this->dynamic_ = ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_SYNCH_2>::instance()->is_dynamic (this);
}

// Default behavior for a ACE_Svc_Handler object is to be registered with
// the ACE_Reactor (thereby ensuring single threading).

template <PR_ST_1, ACE_SYNCH_1> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::open (void *)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::open");
#if defined (DEBUGGING)
  char buf[BUFSIZ];
  ACE_PEER_STREAM_ADDR client_addr;

  if (this->peer_.get_remote_addr (client_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "get_remote_addr"), -1);
    
  if (client_addr.addr_to_string (buf, sizeof buf) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", 
		      "can't obtain peer's address"), -1);

  ACE_DEBUG ((LM_DEBUG, "connected to %s on fd %d\n", 
	     buf, this->peer_.get_handle ()));
#endif /* DEBUGGING */
  if (this->reactor () 
      && this->reactor ()->register_handler 
          (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p", 
		      "unable to register client handler"), -1);
  return 0;
}

// Perform termination activities.

template <PR_ST_1, ACE_SYNCH_1> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::shutdown (void)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::shutdown");

  // Deregister this handler with the ACE_Reactor.
  if (this->reactor ())
    {
      ACE_Reactor_Mask mask = ACE_Event_Handler::WRITE_MASK | 
	                      ACE_Event_Handler::READ_MASK  | 
			      ACE_Event_Handler::DONT_CALL;

      // Make sure there are no timers.
      this->reactor ()->cancel_timer (this);

      // Remove self from reactor.
      this->reactor ()->remove_handler (this, mask);
    }

  this->peer ().close ();
}

template <PR_ST_1, ACE_SYNCH_1> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::dump (void) const
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::dump");
}

template <PR_ST_1, ACE_SYNCH_1> ACE_PEER_STREAM &
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::peer (void) const
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::peer");
  return (ACE_PEER_STREAM &) this->peer_;
}

// Extract the underlying I/O descriptor.

template <PR_ST_1, ACE_SYNCH_1> ACE_HANDLE
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::get_handle (void) const
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::get_handle");
  return this->peer_.get_handle ();
}

// Set the underlying I/O descriptor.

template <PR_ST_1, ACE_SYNCH_1> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::set_handle (ACE_HANDLE h)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::set_handle");
  this->peer_.set_handle (h);
}

template <PR_ST_1, ACE_SYNCH_1> 
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::~ACE_Svc_Handler (void)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::~ACE_Svc_Handler");
  this->shutdown ();
}

template <PR_ST_1, ACE_SYNCH_1> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::handle_close");

  this->destroy ();
  return 0;
}

template <PR_ST_1, ACE_SYNCH_1> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::handle_timeout (const ACE_Time_Value &,
						       const void *)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::handle_timeout");
  return this->handle_close ();
}

template <PR_ST_1, ACE_SYNCH_1> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::close (unsigned long)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::close");
  return this->handle_close ();
}

template <PR_ST_1, ACE_SYNCH_1> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::init (int, char *[])
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::init");
  return -1;
}

template <PR_ST_1, ACE_SYNCH_1> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::fini (void)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::fini");
  return -1;
}

template <PR_ST_1, ACE_SYNCH_1> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::info (char **, size_t) const
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_2>::info");
  return -1;
}
#undef PR_ST_1
#undef PR_ST_2
#endif /* ACE_SVC_HANDLER_C */
