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

#if !defined (ACE_SERVICE_MANAGER_H)
#define ACE_SERVICE_MANAGER_H

#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"
#include "ace/Service_Object.h"

class ACE_Export ACE_Service_Manager : public ACE_Service_Object
  // = TITLE
  //     Provide a standard service that returns a list of all services in the
  //  Service Repository. 
  //
  // = DESCRIPTION
  //
{
public:
  ACE_Service_Manager (void);
  virtual int list_services (void);
  virtual int reconfigure_services (void);

  // = Dynamic linking hooks.
  virtual int init (int argc, char *argv[]);
  virtual int info (char **info_string, size_t length) const;
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
  ACE_SOCK_Acceptor acceptor_;
  int debug_;
  int signum_;
  static u_short DEFAULT_PORT_;
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* _SERVICE_MANAGER_H */
