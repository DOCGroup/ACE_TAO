/* -*- C++ -*- */
// $Id$

/* Handle connections from remote INET connections. */

#ifndef _HANDLE_R_STREAM_H
#define _HANDLE_R_STREAM_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Types.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/INET_Addr.h"

class ACE_Svc_Export Handle_R_Stream : public ACE_Service_Object, public ACE_SOCK_Acceptor
{
public:
  Handle_R_Stream (void);
  virtual int init (int argc, char *argv[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);

private:
  int open (const ACE_INET_Addr &sia, int async = 0);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask);

  ACE_SOCK_Stream new_remote_stream;
  static u_short DEFAULT_PORT;
  static char *login_name;
  static char login[ACE_MAX_USERID];
};

extern ACE_Service_Object_Type rs;

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#include "Handle_R_Stream.i"
#else
#define ACE_INLINE
#endif /* __ACE_INLINE__ */

#endif /* _HANDLE_R_STREAM_H */
