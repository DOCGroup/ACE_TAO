/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//   Svc_Handler.h
//
// = AUTHOR
//    Doug Schmidt and Irfan Pyrarli.
//
// ============================================================================

#ifndef ACE_SVC_HANDLER_H
#define ACE_SVC_HANDLER_H
#include "ace/pre.h"

// Forward decls.
class ACE_Connection_Recycling_Strategy;

#include "ace/Synch_Options.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Service_Config.h"

template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
class ACE_Svc_Handler : public ACE_Task<ACE_SYNCH_USE>
{
  // = TITLE
  //     Defines the interface for a service that exchanges data with
  //     its connected peer.
  //
  // = DESCRIPTION
  //     This class provides a well-defined interface that the
  //     Acceptor and Connector pattern factories use as their target.
  //     Typically, client applications will subclass ACE_Svc_Handler
  //     and do all the interesting work in the subclass.  One thing
  //     that the ACE_Svc_Handler does contain is a PEER_STREAM
  //     endpoint that is initialized by an ACE_Acceptor or
  //     ACE_Connector when a connection is established successfully.
  //     This endpoint is used to exchange data between a
  //     ACE_Svc_Handler and the peer it is connected with.
public:
  // = Initialization and termination methods.
  ACE_Svc_Handler (ACE_Thread_Manager *thr_mgr = 0,
                   ACE_Message_Queue<ACE_SYNCH_USE> *mq = 0,
                   ACE_Reactor *reactor = ACE_Reactor::instance ());
  // Constructor initializes the <thr_mgr> and <mq> by passing them
  // down to the <ACE_Task> base class.  The <reactor> is passed to
  // the <ACE_Event_Handler>.

  virtual ~ACE_Svc_Handler (void);
  // Destructor.

  virtual int open (void * = 0);
  // Activate the client handler.  This is typically called by the
  // <ACE_Acceptor> or <ACE_Connector>.

  virtual int close (u_long flags = 0);
  // Object termination hook -- application-specific cleanup code goes
  // here.

  virtual int idle (u_long flags = 0);
  // Call this method if you want to recycling the <Svc_Handler>
  // instead of closing it.  If the object does not have a recycler,
  // it will be closed.

  virtual ACE_Recyclable_State recycle_state (void) const;
  virtual int recycle_state (ACE_Recyclable_State new_state);
  // Call this method if you want to get/set the state of the
  // <Svc_Handler>.  If the object does not have a recycler, this call
  // will have no effect (and the accessor will return
  // ACE_RECYCLABLE_UNKNOWN).

  virtual void cleanup_hint (void **act_holder = 0);
  // When the svc_handle is no longer needed around as a hint, call
  // this method. In addition, reset <*act_holder> to zero if
  // <act_holder != 0>.

  // = Dynamic linking hooks.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int fini (void);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int info (ACE_TCHAR **info_string, size_t length) const;
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  // = Demultiplexing hooks.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
  // Perform termination activities on the SVC_HANDLER.  The default
  // behavior is to close down the <peer_> (to avoid descriptor leaks)
  // and to <destroy> this object (to avoid memory leaks)!  If you
  // don't want this behavior make sure you override this method...

  virtual int handle_timeout (const ACE_Time_Value &time,
                              const void *);
  // Default behavior when timeouts occur is to close down the
  // <Svc_Handler> by calling <handle_close>.

  virtual ACE_HANDLE get_handle (void) const;
  // Get the underlying handle associated with the <peer_>.

  virtual void set_handle (ACE_HANDLE);
  // Set the underlying handle associated with the <peer_>.

  ACE_PEER_STREAM &peer (void) const;
  // Returns the underlying PEER_STREAM.  Used by
  // <ACE_Acceptor::accept> and <ACE_Connector::connect> factories

  void *operator new (size_t n);
  // Overloaded new operator.  This method unobtrusively records if a
  // <Svc_Handler> is allocated dynamically.

  void * operator new (size_t n,
                       void *p);
  // This operator permits "placement new" on a per-object basis.

  virtual void destroy (void);
  // Call this to free up dynamically allocated <Svc_Handlers>
  // (otherwise you will get memory leaks).  In general, you should
  // call this method rather than <delete> since this method knows
  // whether or not the object was allocated dynamically, and can act
  // accordingly (i.e., deleting it if it was allocated dynamically).

  void operator delete (void *);
  // This really should be private so that users are forced to call
  // <destroy>.  Unfortunately, the C++ standard doesn't allow there
  // to be a public new and a private delete.  It is a bad idea to
  // call this method directly, so use <destroy> instead, unless you
  // know for sure that you've allocated the object dynamically.

#if !defined (ACE_LACKS_PLACEMENT_OPERATOR_DELETE)
  void operator delete (void *, void *);
  // This operator is necessary to complement the class-specific
  // operator new above.  Unfortunately, it's not portable to all C++
  // compilers...
#endif /* ACE_LACKS_PLACEMENT_OPERATOR_DELETE */

  void shutdown (void);
  // Close down the descriptor and unregister from the Reactor

  void dump (void) const;
  // Dump the state of an object.

public:

  // = The following methods are not suppose to be public.

  // Because friendship is *not* inherited in C++, these methods have
  // to be public.

  // = Accessors to set/get the connection recycler.

  virtual void recycler (ACE_Connection_Recycling_Strategy *recycler,
                         const void *recycling_act);
  // Set the recycler and the <recycling_act> that is used during
  // purging and caching.

  virtual ACE_Connection_Recycling_Strategy *recycler (void) const;
  // Get the recycler.

  virtual const void *recycling_act (void) const;
  // Get the recycling act.

  virtual int recycle (void * = 0);
  // Upcall made by the recycler when it is about to recycle the
  // connection.  This gives the object a chance to prepare itself for
  // recycling.  Return 0 if the object is ready for recycling, -1 on
  // failures.

protected:
  ACE_PEER_STREAM peer_;
  // Maintain connection with client.

  int dynamic_;
  // Have we been dynamically created?

  char closing_;
  // Keeps track of whether we are in the process of closing (required
  // to avoid circular calls to <handle_close>).

  ACE_Connection_Recycling_Strategy *recycler_;
  // Pointer to the connection recycler.

  const void *recycling_act_;
  // Asynchronous Completion Token (ACT) to be used to when talking to
  // the recycler.
};

template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
class ACE_Buffered_Svc_Handler : public ACE_Svc_Handler<ACE_PEER_STREAM_2, ACE_SYNCH_USE>
{
  // = TITLE
  //     Defines the interface for a service that exchanges data with
  //     its connected peer and supports buffering.
  //
  // = DESCRIPTION
  //      The buffering feature makes it possible to queue up
  //      <ACE_Message_Blocks> in an <ACE_Message_Queue> until (1) the
  //      queue is "full" or (2) a period of time elapses, at which
  //      point the queue is "flushed" via <sendv_n> to the peer.
public:
  // = Initialization and termination methods.
  ACE_Buffered_Svc_Handler (ACE_Thread_Manager *thr_mgr = 0,
                            ACE_Message_Queue<ACE_SYNCH_USE> *mq = 0,
                            ACE_Reactor *reactor = ACE_Reactor::instance (),
                            size_t max_buffer_size = 0,
                            ACE_Time_Value *relative_timeout = 0);
  // Constructor initializes the <thr_mgr> and <mq> by passing them
  // down to the <ACE_Task> base class.  The <reactor> is passed to
  // the <ACE_Event_Handler>.  The <max_buffer_size> and
  // <relative_timeout> are used to determine at what point to flush
  // the <mq>.  By default, there's no buffering at all.  The
  // <relative_timeout> value is interpreted to be in a unit that's
  // relative to the current time returned by <ACE_OS::gettimeofday>.

  virtual ~ACE_Buffered_Svc_Handler (void);
  // Destructor, which calls <flush>.

  virtual int put (ACE_Message_Block *message_block,
                   ACE_Time_Value *timeout = 0);
  // Insert the <ACE_Message_Block> chain rooted at <message_block>
  // into the <ACE_Message_Queue> with the designated <timeout>.  The
  // <flush> method will be called if this <put> causes the number of
  // bytes to exceed the maximum buffer size or if the timeout period
  // has elapsed.

  virtual int flush (void);
  // Flush the <ACE_Message_Queue>, which writes all the queued
  // <ACE_Message_Block>s to the <PEER_STREAM>.

  virtual int handle_timeout (const ACE_Time_Value &time,
                              const void *);
  // This method is not currently implemented -- this is where the
  // integration with the <Reactor> would occur.

  void dump (void) const;
  // Dump the state of an object.

protected:
  size_t maximum_buffer_size_;
  // Maximum size the <Message_Queue> can be before we have to flush
  // the buffer.

  size_t current_buffer_size_;
  // Current size in bytes of the <Message_Queue> contents.

  ACE_Time_Value next_timeout_;
  // Timeout value used to control when the buffer is flushed.

  ACE_Time_Value interval_;
  // Interval of the timeout.

  ACE_Time_Value *timeoutp_;
  // Timeout pointer.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Svc_Handler.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Svc_Handler.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_SVC_HANDLER_H */
