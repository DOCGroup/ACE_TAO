/* -*- C++ -*- */
// $Id$

#ifndef _SM_CLIENT_H
#define _SM_CLIENT_H

#include "PM_Client.h"
#include "CM_Client.h"
#include "Select_Manager.h"

class SM_Client : public Select_Manager, public CM_Client
{
private:
  PM_Client    *pm_client;

public:
               SM_Client (void);
  virtual int  mux (char *packet, int &packet_length);
  virtual int  demux (char *packet, int &packet_length);
  virtual void process (void);
  virtual     ~SM_Client (void);
};

#ifdef __OPTIMIZE__
inline
SM_Client::SM_Client (void)
{}

inline
SM_Client::~SM_Client (void)
{}

inline void
SM_Client::process (void)
{
  this->pm_client->process ();
}
#endif /* __OPTIMIZE__ */
#endif /* _SM_CLIENT_H */
