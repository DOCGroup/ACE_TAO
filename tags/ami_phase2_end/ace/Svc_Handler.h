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
  ACE_Svc_Handler (ACE_Thread_Manager * = 0,
                   ACE_Message_Queue<ACE_SYNCH_USE> * = 0,
                   ACE_Reactor * = ACE_Reactor::instance ());

  virtual ~ACE_Svc_Handler (void);

  virtual int open (void * = 0);
  // Activate the client handler (called by the ACE_Acceptor or
  // ACE_Connector).

  virtual int close (u_long flags = 0);
  // Object termination hook.

  virtual int idle (u_long flags = 0);
  // Call this method if you want to recycling the <Svc_Handler>
  // instead of closing it.  If the object does not have a recycler,
  // it will be closed.

  virtual void cleanup_hint (void);
  // When the svc_handle is no longer needed around as a hint, call
  // this method.

  // = Dynamic linking hooks.
  virtual int init (int argc, ASYS_TCHAR *argv[]);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int fini (void);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int info (ASYS_TCHAR **info_string, size_t length) const;
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

  void shutdown (void);
  // Close down the descriptor and unregister from the Reactor

  void dump (void) const;
  // Dump the state of an object.

public:

  // Note: The following methods are not suppose to be public.  But
  // because friendship is *not* inherited in C++, these methods have
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

private:
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
  // ACT to be used to when talking to the recycler.
};

#if defined (__ACE_INLINE__)
#include "ace/Svc_Handler.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Svc_Handler.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Svc_Handler.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_SVC_HANDLER_H */
