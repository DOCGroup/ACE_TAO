/* -*- C++ -*- */
// @(#)Handle_Broadcast.h	1.1	10/18/96

/* Handles INET broadcast datagram messages from remote hosts on the local subnet. */

#if !defined (_HANDLE_BROADCAST_H)
#define _HANDLE_BROADCAST_H

#include "ace/Service_Config.h"
#include "ace/Service_Record.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram.h"

class Handle_Broadcast : public ACE_Service_Object, public ACE_SOCK_Dgram
{
public:
  Handle_Broadcast (void);
  ~Handle_Broadcast (void);
  virtual int init (int argc, char *argv[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);

private:
  int open (const ACE_INET_Addr &r, int async = 0);
  virtual int get_handle (void) const;
  virtual int handle_input (int fd);
  virtual int handle_close (int fd, ACE_Reactor_Mask);

  static unsigned short DEFAULT_PORT;
};

extern ACE_Service_Object_Type rb;

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#include "Handle_Broadcast.i"
#else
#define ACE_INLINE 
#endif /* __ACE_INLINE__ */

#endif /* _HANDLE_BROADCAST_H */
