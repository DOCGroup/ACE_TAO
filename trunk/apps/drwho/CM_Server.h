/* -*- C++ -*- */
// $Id$

/* Provides a virtual communcations layer for the server in the drwho
   program. */  

#ifndef _CM_SERVER_H
#define _CM_SERVER_H

#include "Options.h"
#include "ace/OS.h"
#include "global.h"
#include "Comm_Manager.h"

class CM_Server : public Comm_Manager
{
public:
	      CM_Server (void);
  virtual    ~CM_Server (void);

  virtual int open (short port_number);
  virtual int receive (int timeout = 0);
  virtual int send (void);
  virtual int mux (char *packet, int &packet_length)   = 0;
  virtual int demux (char *packet, int &packet_length) = 0;
};

#ifdef __OPTIMIZE__
inline
CM_Server::CM_Server (void)
{}

inline
CM_Server::~CM_Server (void)
{
  if (Options::get_opt (Options::DEBUG))
    fprintf (stderr, "CM_Server\n");
  close (this->sokfd);
}
#endif /* __OPTIMIZE__ */
#endif _CM_SERVER_H
