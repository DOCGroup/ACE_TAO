/* -*- C++ -*- */
// $Id$

// Handles UNIX datagram messages from local host.

#ifndef _HANDLE_L_CODGRAM_H
#define _HANDLE_L_CODGRAM_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Types.h"
#include "ace/LSOCK_CODgram.h"
#include "ace/UNIX_Addr.h"
#include "ace/svc_export.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

class ACE_Svc_Export Handle_L_CODgram : public ACE_Service_Object, public ACE_LSOCK_CODgram
{
public:
  Handle_L_CODgram (void);
  virtual int init (int argc, char *argv[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);

public:
  int open (const ACE_UNIX_Addr &suad, int async = 0);
  virtual ACE_HANDLE get_handle (void) const;
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE, ACE_Reactor_Mask);

  char rendezvous[MAXPATHLEN + 1];
  static const char *DEFAULT_RENDEZVOUS;
};

extern ACE_Service_Object_Type lc;

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#include "Handle_L_CODgram.i"
#else
#define ACE_INLINE
#endif /* __ACE_INLINE__ */

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
#endif /* _HANDLE_L_CODGRAM_H */
