/* -*- C++ -*- */
// $Id$

#ifndef _SMR_CLIENT_H
#define _SMR_CLIENT_H

#include "SM_Client.h"

class SMR_Client : public SM_Client
{
public:
              SMR_Client (short port_number);
  virtual    ~SMR_Client (void);
};

#ifdef __OPTIMIZE__
inline
SMR_Client::SMR_Client (short port_number)
{
  if (CM_Client::open (port_number) < 0)
    perror (Options::program_name), exit (1);
}

inline
SMR_Client::~SMR_Client (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _SMR_CLIENT_H */
