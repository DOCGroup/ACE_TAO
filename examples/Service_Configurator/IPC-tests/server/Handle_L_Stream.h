/* -*- C++ -*- */
// @(#)Handle_L_Stream.h	1.1	10/18/96


/* Handle connections from local UNIX domain sockets. */

#if !defined (_HANDLE_L_STREAM_H)
#define _HANDLE_L_STREAM_H

#include "ace/Service_Config.h"
#include "ace/Service_Record.h"
#include "ace/UNIX_Addr.h"
#include "ace/LSOCK_Acceptor.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

class Handle_L_Stream : public ACE_Service_Object, public ACE_LSOCK_Acceptor
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
  static char *login_name;
  static const char *DEFAULT_RENDEZVOUS;
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


