// $Id$

#ifndef ACE_SVC_HANDLER_C
#define ACE_SVC_HANDLER_C

#define ACE_BUILD_DLL
#include "ace/Svc_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Object_Manager.h"
#include "ace/Strategies.h"

#if !defined (__ACE_INLINE__)
#include "ace/Svc_Handler.i"
#endif /* __ACE_INLINE__ */

#include "ace/Dynamic.h"

ACE_RCSID(ace, Svc_Handler, "$Id$")

#define PR_ST_1 ACE_PEER_STREAM_1
#define PR_ST_2 ACE_PEER_STREAM_2

template <PR_ST_1, ACE_SYNCH_DECL> void *
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::operator new (size_t n)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::operator new");

  ACE_Dynamic *const dynamic_instance = ACE_Dynamic::instance ();

  if (dynamic_instance == 0)
    {
      // If this ACE_ASSERT fails, it may be due to running of out TSS
      // keys.  Try using ACE_HAS_TSS_EMULATION, or increasing
      // ACE_DEFAULT_THREAD_KEYS if already using TSS emulation.
      ACE_ASSERT (dynamic_instance != 0);

      return 0;
    }
  else
    {
      // Allocate the memory and store it (usually in thread-specific
      // storage, depending on config flags).
      dynamic_instance->set ();

      return ::new char[n];
    }
}

template <PR_ST_1, ACE_SYNCH_DECL> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::destroy (void)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::destroy");

  // Only delete ourselves if we're not owned by a module and have
  // been allocated dynamically.
  if (this->mod_ == 0 && this->dynamic_ && this->closing_ == 0)
    // Will call the destructor, which automatically calls <shutdown>.
    // Note that if we are *not* allocated dynamically then the
    // destructor will call <shutdown> automatically when it gets run
    // during cleanup.
    delete this;
}

template <PR_ST_1, ACE_SYNCH_DECL> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::operator delete (void *obj)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::delete");
  // You cannot delete a 'void*' (X3J16/95-0087 5.3.5.3), but we know
  // the pointer was created using new char[] (see operator new code),
  // so we use a cast:
  char *tmp = (char *) obj;
  ::delete [] tmp;
}

// Default constructor.

template <PR_ST_1, ACE_SYNCH_DECL>
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::ACE_Svc_Handler (ACE_Thread_Manager *tm,
                                                          ACE_Message_Queue<ACE_SYNCH_USE> *mq,
                                                          ACE_Reactor *reactor)
  : ACE_Task<ACE_SYNCH_USE> (tm, mq),
    closing_ (0),
    recycler_ (0),
    recycling_act_ (0)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::ACE_Svc_Handler");

  this->reactor (reactor);

  // This clever idiom transparently checks if we were allocated
  // dynamically.  This information is used by the <destroy> method to
  // decide if we need to delete <this>...  The idiom is based on a
  // paper by Michael van Rooyen (mrooyen@cellnet.co.uk) that appeared
  // in the April '96 issue of the C++ Report.  We've spruced it up to
  // work correctly in multi-threaded programs by using our ACE_TSS
  // class.
  this->dynamic_ = ACE_Dynamic::instance ()->is_dynamic ();
  if (this->dynamic_)
    // Make sure to reset the flag
    ACE_Dynamic::instance ()->reset ();
}

// Default behavior for a ACE_Svc_Handler object is to be registered with
// the ACE_Reactor (thereby ensuring single threading).

template <PR_ST_1, ACE_SYNCH_DECL> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::open (void *)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::open");
#if defined (DEBUGGING)
  ASYS_TCHAR buf[BUFSIZ];
  ACE_PEER_STREAM_ADDR client_addr;

  if (this->peer_.get_remote_addr (client_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("get_remote_addr")),
                      -1);
  else if (client_addr.addr_to_string (buf, sizeof buf) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("can't obtain peer's address")),
                      -1);
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("connected to %s on fd %d\n"),
              buf,
              this->peer_.get_handle ()));
#endif /* DEBUGGING */
  if (this->reactor ()
      && this->reactor ()->register_handler
      (this,
       ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("unable to register client handler")),
                      -1);
  return 0;
}

// Perform termination activities.

template <PR_ST_1, ACE_SYNCH_DECL> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::shutdown (void)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::shutdown");

  // Deregister this handler with the ACE_Reactor.
  if (this->reactor ())
    {
      ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK |
        ACE_Event_Handler::DONT_CALL;

      // Make sure there are no timers.
      this->reactor ()->cancel_timer (this);

      // Remove self from reactor.
      this->reactor ()->remove_handler (this, mask);
    }

  // Remove self from the recycler.
  if (this->recycler ())
    this->recycler ()->purge (this->recycling_act_);

  this->peer ().close ();
}

template <PR_ST_1, ACE_SYNCH_DECL> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::cleanup_hint (void)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::cleanup_hint");
  // Remove as hint.
  if (this->recycler ())
    this->recycler ()->cleanup_hint (this->recycling_act_);

}

template <PR_ST_1, ACE_SYNCH_DECL> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::dump (void) const
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::dump");
}

template <PR_ST_1, ACE_SYNCH_DECL> ACE_PEER_STREAM &
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::peer (void) const
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::peer");
  return (ACE_PEER_STREAM &) this->peer_;
}

// Extract the underlying I/O descriptor.

template <PR_ST_1, ACE_SYNCH_DECL> ACE_HANDLE
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::get_handle (void) const
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::get_handle");
  return this->peer_.get_handle ();
}

// Set the underlying I/O descriptor.

template <PR_ST_1, ACE_SYNCH_DECL> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::set_handle (ACE_HANDLE h)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::set_handle");
  this->peer_.set_handle (h);
}

template <PR_ST_1, ACE_SYNCH_DECL>
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::~ACE_Svc_Handler (void)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::~ACE_Svc_Handler");

  if (this->closing_ == 0)
    {
      // We're closing down now, so make sure not to call ourselves
      // recursively via other calls to handle_close() (e.g., from the
      // Timer_Queue).
      this->closing_ = 1;

      this->shutdown ();
    }
}

template <PR_ST_1, ACE_SYNCH_DECL> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::handle_close (ACE_HANDLE,
                                                       ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::handle_close");

  this->destroy ();
  return 0;
}

template <PR_ST_1, ACE_SYNCH_DECL> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::handle_timeout (const ACE_Time_Value &,
                                                         const void *)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::handle_timeout");
  return this->handle_close ();
}

template <PR_ST_1, ACE_SYNCH_DECL> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::close (unsigned long)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::close");
  return this->handle_close ();
}

template <PR_ST_1, ACE_SYNCH_DECL> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::init (int argc, ASYS_TCHAR *argv[])
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::init");
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  return -1;
}

template <PR_ST_1, ACE_SYNCH_DECL> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::fini (void)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::fini");
  return -1;
}

template <PR_ST_1, ACE_SYNCH_DECL> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::info (ASYS_TCHAR **, size_t) const
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::info");
  return -1;
}

template <PR_ST_1, ACE_SYNCH_DECL> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::idle (u_long flags)
{
  if (this->recycler ())
    return this->recycler ()->cache (this->recycling_act_);
  else
    return this->close (flags);
}

template <PR_ST_1, ACE_SYNCH_DECL> void
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::recycler (ACE_Connection_Recycling_Strategy *recycler,
                                                   const void *recycling_act)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::recycler");
  this->recycler_ = recycler;
  this->recycling_act_ = recycling_act;
}

template <PR_ST_1, ACE_SYNCH_DECL> ACE_Connection_Recycling_Strategy *
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::recycler (void) const
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::recycler");
  return this->recycler_;
}

template <PR_ST_1, ACE_SYNCH_DECL> const void *
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::recycling_act (void) const
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::recycling_act");
  return this->recycling_act_;
}

template <PR_ST_1, ACE_SYNCH_DECL> int
ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::recycle (void *)
{
  ACE_TRACE ("ACE_Svc_Handler<PR_ST_2, ACE_SYNCH_USE>::recycle");
  // By default, the object is ready and willing to be recycled.
  return 0;
}

#undef PR_ST_1
#undef PR_ST_2
#endif /* ACE_SVC_HANDLER_C */
