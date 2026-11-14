/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Comm_Manager.h
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================

#ifndef _COMM_MANAGER_H
#define _COMM_MANAGER_H

#include "global.h"
#include "ace/os_include/netinet/os_in.h"

/**
 * @brief Provides the virtual communication layer for the drwho program.
 */
class Comm_Manager
{
public:
  virtual ~Comm_Manager ();
protected:
  char recv_packet_[UDP_PACKET_SIZE];
  char send_packet_[UDP_PACKET_SIZE];
  sockaddr_in sin_ {};
  ACE_HANDLE sokfd_ {ACE_INVALID_HANDLE};

  virtual int mux (char *packet, int &packet_length)   = 0;
  virtual int demux (char *packet, int &packet_length) = 0;
  virtual int open (short port_number) = 0;
  virtual int receive (int timeout = 0) = 0;
  virtual int send () = 0;
};

#endif /* _COMM_MANAGER_H */
