/* -*- C++ -*- */
// $Id$

#ifndef _SML_CLIENT_H
#define _SML_CLIENT_H

#include "SM_Client.h"
#include "SML_Server.h"

class SML_Client : public SM_Client
{
private:
  SML_Server  sml_server;
  int	      packet_length;

public:
              SML_Client (void);
  virtual    ~SML_Client (void);
  virtual int receive (int timeout = 0);
  virtual int send (void);
};

#ifdef __OPTIMIZE__
inline
SML_Client::SML_Client (void)
{}

inline
SML_Client::~SML_Client (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _SML_CLIENT_H */
