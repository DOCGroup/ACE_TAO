/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    CM_Server.h
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
  CM_Server () = default;
  ~CM_Server () override;

  int open (short port_number) override;
  int receive (int timeout = 0) override;
  int send () override;
  int mux (char *packet, int &packet_length) override = 0;
  int demux (char *packet, int &packet_length) override = 0;
};

#endif /* _CM_SERVER_H */
