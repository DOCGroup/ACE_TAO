// -*- C++ -*-
//
// $Id$

// Handle connections from local UNIX domain sockets.

#ifndef _HANDLE_L_STREAM_H
#define _HANDLE_L_STREAM_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Types.h"
#include "ace/UNIX_Addr.h"
#include "ace/LSOCK_Acceptor.h"
#include "ace/svc_export.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

class ACE_Svc_Export Handle_L_Stream : public ACE_Service_Object, public ACE_LSOCK_Acceptor
{
public:
  Handle_L_Stream (void);
  ~Handle_L_Stream (void);
  virtual int init (int argc, char *argv[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);

private:
  int open (const ACE_UNIX_Addr &suas, int async = 0);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask);

  char rendezvous[MAXPATHLEN + 1];
  static const char *DEFAULT_RENDEZVOUS;
  static char *login_name;
  static char login[ACE_MAX_USERID];
};

extern ACE_Service_Object_Type ls;

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#include "Handle_L_Stream.i"
#else
#define ACE_INLINE
#endif /* __ACE_INLINE__ */
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#endif /* _HANDLE_L_STREAM_H */
