// $Id$

/*! \file netlink_pingpong.cpp
    \brief a ping pong netlink tester
    
    Why use netlink sockets as a communication facilty bettwen kernel to user space application ?
    
   1. netlink sockets are used all over the kernel which means that this technology is tested.
   2. easy to use.
   3. The netlink is non-reliable protocol. Though it seems funny, the last thing you need when your system 
       is under heavy load is retransmission and new allocations.
   4. The socket and the socket buffer ( sk_buff ) interfaces supply you a handfull of utilties. ( enquee,dequeu..).
   5. As fat as i know, the user space interface remained unchanged from 2.4 to 2.6 .
   
   One bad thing I've noticed about it is that the buffers are reallocted in the kernel in sending. 
   look at line 670 in lxr at af_netlink.c in kernel 2.6.5.    
   
*/

#include <iostream>
#include <cstdlib>
#include "netlinkgateway.h"

using namespace std;

int main(int argc, char *argv[])
{
  cout << "Hello, world! for netlink ping pong reactor " << endl;
  if ( 0>CNetlinkGateway::instance()->open()   ){
       cout << "failed to open netlinkgateway"  << endl;
       return -1;
  }

  CNetlinkGateway::instance()->reactor(ACE_Reactor::instance ());
  CNetlinkGateway::instance()->Start();
  ACE_Reactor::instance ()->run_reactor_event_loop ();
  return 0;
}
