/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Service_Manager.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SERVICE_MANAGER_H
#define ACE_SERVICE_MANAGER_H

#include "ace/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/Service_Object.h"

class ACE_Export ACE_Service_Manager : public ACE_Service_Object
{
  // = TITLE
  //     Provide a standard ACE service for managing all the services
  //     configured in an <ACE_Service_Repository>.
  //
  // = DESCRIPTION
  //     This implementation is very simple.  It just handles each
  //     client request one at a time.  Each request is associated
  //     with a new connection, which is closed when the request is
  //     processed.  In addition, you must be using the singleton
  //     <ACE_Reactor::instance> in order to trigger reconfigurations.
  //     This scheme can certainly be improved.
public:
  // = Initialization and termination hooks.
  ACE_Service_Manager (void);
  // Constructor.

  ~ACE_Service_Manager (void);
  // Destructor.

protected:
  // = Perform the various meta-services.
  virtual int reconfigure_services (void);
  // Trigger a remote reconfiguration of the Service Configurator.

  virtual int list_services (void);
  // Determine all the services offered by this daemon and return the
  // information back to the client.

  // = Dynamic linking hooks.
  virtual int init (int argc, ASYS_TCHAR *argv[]);
  virtual int info (ASYS_TCHAR **info_string, size_t length) const;
  virtual int fini (void);

  // = Scheduling hooks.
  virtual int suspend (void);
  virtual int resume (void);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int open (const ACE_INET_Addr &sia);

  // = Demultiplexing hooks.
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask);
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);

  ACE_SOCK_Stream client_stream_;
  // Connection to the client (we only support one client connection
  // at a time).

  ACE_SOCK_Acceptor acceptor_;
  // Acceptor instance.

  int debug_;
  // Keep track of the debugging level.

  int signum_;
  // The signal used to trigger reconfiguration.

  static u_short DEFAULT_PORT_;
  // Default port for the Acceptor to listen on.
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* _SERVICE_MANAGER_H */
