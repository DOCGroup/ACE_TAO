/* -*- C++ -*- */
// $Id$

#ifndef _SMR_SERVER_H
#define _SMR_SERVER_H

#include "SM_Server.h"

class SMR_Server : public SM_Server
{
public:
              SMR_Server (short port_number);
	      ~SMR_Server (void);
};

#ifdef __OPTIMIZE__
inline
SMR_Server::SMR_Server (short port_number)
{
  if (CM_Server::open (port_number) < 0)
    perror (Options::program_name), exit (1);
}

inline
SMR_Server::~SMR_Server (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _SMR_SERVER_H */
