/* -*- C++ -*- */
// $Id$

/* Handle connections from local UNIX ACE_FIFO */
/* Read from a well known ACE_FIFO and write to stdout. */

#if !defined (_HANDLE_L_FIFO_H)
#define _HANDLE_L_FIFO_H

#include "ace/Service_Config.h"
#include "ace/Service_Record.h"
#include "ace/FIFO_Recv_Msg.h"

class Handle_L_FIFO : public ACE_Service_Object, public ACE_FIFO_Recv_Msg
{
public:	
  Handle_L_FIFO (void);
  virtual int init (int argc, char *argv[]);
  virtual int info (char **, size_t) const;
  virtual int fini (void);

private:
  int open (const char *rendezvous_fifo);
  virtual int get_handle (void) const;
  virtual int handle_input (int fd);
  virtual int handle_close (int fd, ACE_Reactor_Mask);

  static const char *DEFAULT_RENDEZVOUS;
};

extern ACE_Service_Object_Type lf;

#if defined (__ACE_INLINE__)
#define ACE_INLINE inline
#include "Handle_L_FIFO.i"
#else
#define ACE_INLINE 
#endif /* __ACE_INLINE__ */

#endif /* _HANDLE_L_FIFO_H */


