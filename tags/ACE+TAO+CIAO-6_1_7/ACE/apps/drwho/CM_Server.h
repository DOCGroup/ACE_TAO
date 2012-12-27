/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    CM_Server.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _CM_SERVER_H
#define _CM_SERVER_H

#include "Options.h"
#include "global.h"
#include "Comm_Manager.h"

/**
 * @class CM_Server
 *
 * @brief Provides a virtual communcations layer for the server in drwho.
 */
class CM_Server : public Comm_Manager
{
public:
  CM_Server (void);
  virtual ~CM_Server (void);

  virtual int open (short port_number);
  virtual int receive (int timeout = 0);
  virtual int send (void);
  virtual int mux (char *packet, int &packet_length)   = 0;
  virtual int demux (char *packet, int &packet_length) = 0;
};

#endif /* _CM_SERVER_H */
