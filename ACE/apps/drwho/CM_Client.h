/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    CM_Client.h
 *
 *  Provides a virtual communcations layer for the client in the
 *  drwho program.
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef _CM_CLIENT_H
#define _CM_CLIENT_H

#include "Comm_Manager.h"
#include "ace/Time_Value.h"

/**
 * @class CM_Client
 *
 * @brief Provides a virtual communcations layer for the client in the
 * drwho program.
 */
class CM_Client : public Comm_Manager
{
public:
  CM_Client ();

  virtual ~CM_Client ();

  int mux (char *packet, int &packet_length) override = 0;
  int demux (char *packet, int &packet_length) override = 0;
  int open (short port_number) override;
  int receive (int timeout = 0) override;
  int send () override;

private:
  fd_set read_fd_;
  ACE_Time_Value time_out_;
  ACE_Time_Value *top_;
};

#endif /* _CM_CLIENT_H */
