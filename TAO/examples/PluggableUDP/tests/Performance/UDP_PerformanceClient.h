// -*- C++ -*-

//=============================================================================
/**
 *  @file    UDP_PerformanceClient.h
 *
 *  This class implements the client functionality of the UDP test.
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#ifndef UDP_PERFORMANCECLIENT_H
#define UDP_PERFORMANCECLIENT_H

#include "ace/Task.h"
#include "tao/ORB.h"

#include "UDP_i.h"
#include "UDPC.h"

/// UDP client task that wraps the client functionality in one class.
class UDP_PerformanceClient
  : public ACE_Task_Base
{
public:
  UDP_PerformanceClient (CORBA::ORB_ptr orb,
                         UDP_ptr udp,
                         UDP_i &udpHandler,
                         ACE_UINT32 burst_messages);

  ~UDP_PerformanceClient () override = default;

  UDP_PerformanceClient (UDP_PerformanceClient const &) = delete;
  UDP_PerformanceClient &operator= (UDP_PerformanceClient const &) = delete;
  UDP_PerformanceClient (UDP_PerformanceClient &&) = delete;
  UDP_PerformanceClient &operator= (UDP_PerformanceClient &&) = delete;

private:
  /// Runs the client in a separate thread.
  int svc () override;

private:
  CORBA::ORB_var orb_ {};

  UDP_var udp_ {};

  UDP_i &udpHandler_;

  ACE_UINT32 last_wrong_messages_ {};

  ACE_UINT32 burst_messages_ {};
};

#endif /* UDP_PERFORMANCECLIENT_H */
