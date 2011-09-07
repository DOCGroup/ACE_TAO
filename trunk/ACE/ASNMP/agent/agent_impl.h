/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    agent_impl.h
 *
 *  $Id$
 *
 *  @author Michael R. MacFaden
 */
//=============================================================================


#ifndef AGENT_IMPL_H
#define AGENT_IMPL_H

#include <ace/Event_Handler.h>
#include <ace/SOCK_Dgram.h>
#include <ace/INET_Addr.h>
#include <ace/High_Res_Timer.h>
#include <asnmp/sagent.h>
#include <asnmp/vb.h>

class agent_impl : public sagent
{
public:
  agent_impl(unsigned short port, const char *rd, const char *wr);
  virtual ~agent_impl();

  /// retrieve data from a peer agent for a given list of oid values
  virtual int handle_get( Pdu &pdu, UdpTarget &target);

  /// retrieve data lexically adjacent to the oids specified in the pdu
  /// from the peer agent
  virtual int handle_get_next( Pdu &pdu, UdpTarget &target);

  /// set data in the agent from the list of oids in the pdu
  virtual int handle_set( Pdu &pdu, UdpTarget &target);

  /// main loop here using a reactor object
  int process_requests();

private:
  /// this agent's read, write community strings
  UdpTarget tgt_;
  /// set values for a "get" cmd
  int get_response(Vb& vb);
  /// agent "uptime" ticks reported in 1/100 second
  ACE_High_Res_Timer agent_clock_;
};

#endif /* AGENT_IMPL_H */
