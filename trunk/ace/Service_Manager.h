/* -*- C++ -*- */


//=============================================================================
/**
 *  @file    Service_Manager.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef ACE_SERVICE_MANAGER_H
#define ACE_SERVICE_MANAGER_H
#include "ace/pre.h"

#include "ace/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/Service_Object.h"

/**
 * @class ACE_Service_Manager
 *
 * @brief Provide a standard ACE service for managing all the services
 * configured in an <ACE_Service_Repository>.
 *
 * This implementation is very simple.  It just handles each
 * client request one at a time.  Each request is associated
 * with a new connection, which is closed when the request is
 * processed.  In addition, you must be using the singleton
 * <ACE_Reactor::instance> in order to trigger reconfigurations.
 * This scheme can certainly be improved.
 */
class ACE_Export ACE_Service_Manager : public ACE_Service_Object
{
public:
  // = Initialization and termination hooks.
  /// Constructor.
  ACE_Service_Manager (void);

  /// Destructor.
  ~ACE_Service_Manager (void);

protected:
  // = Perform the various meta-services.
  /// Trigger a remote reconfiguration of the Service Configurator.
  virtual int reconfigure_services (void);

  /// Determine all the services offered by this daemon and return the
  /// information back to the client.
  virtual int list_services (void);

  // = Dynamic linking hooks.
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int info (ACE_TCHAR **info_string, size_t length) const;
  virtual int fini (void);

  // = Scheduling hooks.
  virtual int suspend (void);
  virtual int resume (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  int open (const ACE_INET_Addr &sia);

  // = Demultiplexing hooks.
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask);
  virtual int handle_signal (int signum, siginfo_t *, ucontext_t *);

  /// Connection to the client (we only support one client connection
  /// at a time).
  ACE_SOCK_Stream client_stream_;

  /// Acceptor instance.
  ACE_SOCK_Acceptor acceptor_;

  /// Keep track of the debugging level.
  int debug_;

  /// The signal used to trigger reconfiguration.
  int signum_;

  /// Default port for the Acceptor to listen on.
  static u_short DEFAULT_PORT_;
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* _SERVICE_MANAGER_H */
