// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/PluggableUDP/tests/Performance
//
// = FILENAME
//    UDP_PerformanceClient.h
//
// = DESCRIPTION
//    This class implements the client functionality of the UDP test.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#ifndef UDP_PERFORMANCECLIENT_H
#define UDP_PERFORMANCECLIENT_H

#include "ace/Task.h"
#include "tao/ORB.h"

#include "UDP_i.h"
#include "UDPC.h"

class UDP_PerformanceClient
  : public ACE_Task_Base
{
  // = TITLE
  //    UDP Client interface subclass.
  //
  // = DESCRIPTION
  //    This class wrapps the client functionality into one class
  //
public:
  // = Initialization and termination methods.
  UDP_PerformanceClient (CORBA::ORB_ptr orb,
                         UDP_ptr udp,
                         UDP_i *udpHandler,
                         ACE_UINT32 burst_messages);
  // Constructor

  virtual ~UDP_PerformanceClient (void);
  // Destructor

private:
  // Runs in a separate thread.
  virtual int svc (void );

private:
  CORBA::ORB_var orb_;

  UDP_var udp_;

  UDP_i *udpHandler_;

  ACE_UINT32 last_wrong_messages_;

  ACE_UINT32 burst_messages_;
};

#endif /* UDP_PERFORMANCECLIENT_H */
