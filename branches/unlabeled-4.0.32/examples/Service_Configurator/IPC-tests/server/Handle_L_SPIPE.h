/* -*- C++ -*- */
// @(#)Handle_L_SPIPE.h	1.1	10/18/96

/* Handle connections from local UNIX ACE_SPIPE */
/* Read from a well known ACE_SPIPE and write to stdout. */

#if !defined (_HANDLE_L_SPIPE_H)
#define _HANDLE_L_SPIPE_H

#include "ace/Service_Config.h"
#include "ace/Service_Record.h"
#include "ace/SPIPE_Acceptor.h"

#if defined (ACE_HAS_STREAM_PIPES)

class Handle_L_SPIPE : public ACE_Service_Object, public ACE_SPIPE_Acceptor
{
public:	
  Handle_L_SPIPE (void);
  virtual int init (int argc, char *argv[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);

private:
  int open (const ACE_SPIPE_Addr &rendezvous_spipe);
  virtual int get_handle (void) const;
  virtual int handle_input (int fd);
  virtual int handle_close (int fd, ACE_Reactor_Mask);

  static const char *DEFAULT_RENDEZVOUS;
};

extern ACE_Service_Object_Type lsp;

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#include "Handle_L_SPIPE.i"
#else
#define ACE_INLINE 
#endif /* __ACE_INLINE__ */

#endif /* _HANDLE_L_SPIPE_H */


#endif /* ACE_HAS_STREAM_PIPES */
