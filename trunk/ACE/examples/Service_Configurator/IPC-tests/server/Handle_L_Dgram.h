// -*- C++ -*-
// $Id$

// Handles UNIX datagram messages from local host.

#ifndef _HANDLE_L_DGRAM_H
#define _HANDLE_L_DGRAM_H

#include "ace/Service_Config.h"
#include "ace/Reactor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Types.h"
#include "ace/LSOCK_Dgram.h"
#include "ace/UNIX_Addr.h"
#include "ace/svc_export.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

class ACE_Svc_Export Handle_L_Dgram : public ACE_Service_Object, public ACE_LSOCK_Dgram
{
public:
  Handle_L_Dgram (void);
  virtual int init (int argc, ACE_TCHAR *argv[]);
  virtual int info (ACE_TCHAR **, size_t) const;
  virtual int fini (void);

private:
  int open (const ACE_UNIX_Addr &suad, int async = 0);
  virtual int get_handle (void) const;
  virtual int handle_input (int fd);
  virtual int handle_close (int fd, ACE_Reactor_Mask);

  ACE_TCHAR rendezvous[MAXPATHLEN + 1];
  static const ACE_TCHAR *DEFAULT_RENDEZVOUS;
};

extern ACE_Service_Object_Type ld;

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#include "Handle_L_Dgram.i"
#else
#define ACE_INLINE
#endif /* __ACE_INLINE__ */

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#endif /* _HANDLE_L_DGRAM_H */
