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

#if !defined (ACE_SVC_HANDLER_H)
#define ACE_SVC_HANDLER_H

#include "ace/Synch_Options.h"
#include "ace/Task.h"
#include "ace/Service_Config.h"
#include "ace/Synch_T.h"

// Forward decls.
class ACE_Dynamic;

template <ACE_PEER_STREAM_1, ACE_SYNCH_1>
class ACE_Svc_Handler : public ACE_Task<ACE_SYNCH_2>
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
{
public:
  // = Initialization and termination methods.
  ACE_Svc_Handler (ACE_Thread_Manager * = 0, 
		   ACE_Message_Queue<ACE_SYNCH_2> * = 0,
		   ACE_Reactor * = ACE_Service_Config::reactor ());

  virtual ~ACE_Svc_Handler (void); 

  virtual int open (void * = 0);
  // Activate the client handler (called by the ACE_Acceptor or
  // ACE_Connector).

  virtual int close (u_long flags = 0);
  // Object termination hook.

  // = Dynamic linking hooks.
  virtual int init (int argc, char *argv[]);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int fini (void);
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  virtual int info (char **info_string, size_t length) const;
  // Default version does no work and returns -1.  Must be overloaded
  // by application developer to do anything meaningful.

  // = Demultiplexing hooks.

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);
  // Perform termination activities on the SVC_HANDLER.  The default
  // behavior is to close down the <peer_> (to avoid descriptor leaks)
  // and to delete this (to avoid memory leaks)!  If you don't want
  // this behavior make sure you override this method...

  virtual int handle_timeout (const ACE_Time_Value &time,
			      const void *);
  // Default behavior when timeouts occur is to close down the
  // <Svc_Handler> by calling <handle_close>.

  virtual ACE_HANDLE get_handle (void) const;
  // Get the underlying handle associated with the <peer_>.

  virtual void set_handle (ACE_HANDLE); 
  // Set the underlying handle associated with the <peer_>.

  ACE_PEER_STREAM &peer (void) const;
  // Returns the underlying PEER_STREAM

  // operator ACE_PEER_STREAM &();
  // Returns the underlying PEER_STREAM (used by
  // ACE_Acceptor::accept() and ACE_Connector::connect() factories).

  virtual void destroy (void);
  // Call this instead of <delete> to free up dynamically allocated
  // <Svc_Handlers> (otherwise you will get memory leaks).  This
  // method knows whether or not the object was allocated dynamically,
  // and can act accordingly (i.e., deleting it if it was allocated
  // dynamically, otherwise ignoring it).

  void *operator new (size_t n);
  // Overloaded new operator.  This is used to unobtrusively detect
  // when a Svc_Handler is allocated dynamically.

  void shutdown (void);
  // Close down the descriptor and unregister from the Reactor

  void dump (void) const;
  // Dump the state of an object.

public:

  void operator delete (void *);
  // This really should be private so that users are forced to call
  // destroy().  Unfortunately, the C++ standard doesn't allow there
  // to be a public new and a private delete.

private:  
  ACE_PEER_STREAM peer_; 
  // Maintain connection with client.

  static ACE_Dynamic *instance (void);
  // Point of access to the singleton.

  char dynamic_;
  // Have we been dynamically created?

  char closing_;
  // Keeps track of whether we are in the process of closing (required
  // to avoid circular calls to <handle_close>).

#if defined (ACE_MT_SAFE) && !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES)
  static ACE_Thread_Mutex ace_svc_handler_lock_;
  // Lock the creation of the Singleton.
#endif /* defined (ACE_MT_SAFE) && !defined (ACE_LACKS_STATIC_DATA_MEMBER_TEMPLATES) */
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
