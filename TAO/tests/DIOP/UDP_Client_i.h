// -*- C++ -*-

//=============================================================================
/**
 *  @file    UDP_Client_i.h
 *
 *  This class implements the client functionality of the UDP test.
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#ifndef UDP_CLIENT_I_H
#define UDP_CLIENT_I_H

#include "ace/Task.h"
#include "tao/ORB.h"
#include <cstddef>

#include "UDPC.h"

class UDP_Client_i
: public ACE_Task_Base
{
  // = TITLE
  //    UDP Client interface subclass.
  //
  // = DESCRIPTION
  //    This class wrapps the client functionality into one class
  //
public:
  /// Constructor
  UDP_Client_i (CORBA::ORB_ptr orb,
                UDP_ptr udp,
                UDP_ptr udpHandler,
                unsigned int msec,
                std::size_t iterations);
  UDP_Client_i (UDP_Client_i const &) = delete;
  UDP_Client_i (UDP_Client_i &&) = delete;
  UDP_Client_i &operator= (UDP_Client_i const &) = delete;
  UDP_Client_i &operator= (UDP_Client_i &&) = delete;

  /// Destructor
  ~UDP_Client_i () override = default;

private:
  // Runs in a separate thread.
  int svc () override;

private:
  CORBA::ORB_var const orb_;

  UDP_var const udp_;

  UDP_var udpHandler_;

  unsigned int delay_;

  std::size_t iterations_;
};


#endif /* UDP_CLIENT_I_H */
