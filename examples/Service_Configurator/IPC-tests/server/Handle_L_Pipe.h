// -*- C++ -*-
// $Id$

// Handle connections from local UNIX domain sockets that are sending
// end-points from a pipe!

#ifndef _HANDLE_L_PIPE_H
#define _HANDLE_L_PIPE_H

#include "ace/Service_Config.h"
#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Types.h"
#include "ace/UNIX_Addr.h"
#include "ace/LSOCK_Acceptor.h"
#include "ace/svc_export.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

class ACE_Svc_Export Handle_L_Pipe : public ACE_Service_Object, public ACE_LSOCK_Acceptor
{
public:
  Handle_L_Pipe (void);
  ~Handle_L_Pipe (void);
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int info (ACE_TCHAR **, size_t) const;
  virtual int fini (void);

private:
  int open (const ACE_UNIX_Addr &suap, int async = 0);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask);

  char *upper_case (char s[], int n);

  ACE_TCHAR rendezvous[MAXPATHLEN + 1];
  static const ACE_TCHAR *DEFAULT_RENDEZVOUS;
};

extern ACE_Service_Object_Type lp;

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#include "Handle_L_Pipe.inl"
#else
#define ACE_INLINE
#endif /* __ACE_INLINE__ */

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#endif /* _HANDLE_L_PIPE_H */
