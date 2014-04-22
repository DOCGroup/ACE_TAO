/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    CM_Client.h
 *
 *  $Id$
 *
 *  Provides a virtual communcations layer for the client in the
 *  drwho program.
 *
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
  // = Initialization and termination.
  /// Constructor.
  CM_Client (void);

  /// Destructor.
  virtual ~CM_Client (void);

  virtual int mux (char *packet, int &packet_length)   = 0;
  virtual int demux (char *packet, int &packet_length) = 0;
  virtual int open (short port_number);
  virtual int receive (int timeout = 0);
  virtual int send (void);

private:
  fd_set read_fd_;
  ACE_Time_Value time_out_;
  ACE_Time_Value *top_;
};

#endif /* _CM_CLIENT_H */
