/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    agent_impl.h
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
  agent_impl (unsigned short port, char const *rd, char const *wr);
  ~agent_impl () override;
  agent_impl (agent_impl const &) = delete;
  agent_impl (agent_impl &&) = delete;
  agent_impl &operator= (agent_impl const &) = delete;
  agent_impl &operator= (agent_impl &&) = delete;

  /// retrieve data from a peer agent for a given list of oid values
  int handle_get (Pdu &pdu, UdpTarget &target) override;

  /// retrieve data lexically adjacent to the oids specified in the pdu
  /// from the peer agent
  int handle_get_next (Pdu &pdu, UdpTarget &target) override;

  /// set data in the agent from the list of oids in the pdu
  int handle_set (Pdu &pdu, UdpTarget &target) override;

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
