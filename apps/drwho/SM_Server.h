/* -*- C++ -*- */
// $Id$

#ifndef _SM_SERVER_H
#define _SM_SERVER_H

#include "PM_Server.h"
#include "CM_Server.h"
#include "Select_Manager.h"

class SM_Server : public Select_Manager, public CM_Server
{
private:
  PM_Server   *pm_server;

public:
              SM_Server (void);
  virtual    ~SM_Server (void);
  virtual int mux (char *packet, int &packet_length);
  virtual int demux (char *packet, int &packet_length);
};

#ifdef __OPTIMIZE__
inline
SM_Server::SM_Server (void)
{}

inline
SM_Server::~SM_Server (void)
{}
#endif /* __OPTIMIZE__ */
#endif /* _SM_SERVER_H */
