// implement a prototype SNMP Agent using ASNMP and ACE

#include <ace/Reactor.h>
#include <ace/SOCK_Dgram.h>
#include <ace/INET_Addr.h>
#include <ace/Signal.h>

#include "agent_impl.h"

agent_impl::agent_impl (unsigned short port, char const *rd, char const *wr)
  : sagent (port)
{
  ACE_TRACE ("agent_impl::agent_impl");
  tgt_.set_read_community (rd);
  tgt_.set_write_community (wr);
  agent_clock_.start ();
}

agent_impl::~agent_impl ()
{
  ACE_TRACE ("agent_impl::~agent_impl");
}

// callback : have received a Pdu from the target host with given read comm str
// this is really simplistic, but gives the general idea
int agent_impl::handle_get (Pdu &pdu, UdpTarget &target)
{
  ACE_TRACE ("agent_impl::handle_get");
  OctetStr mgr_rd_str {}, agent_rd_str {};
  target.get_read_community (mgr_rd_str); // requester's read community string
  tgt_.get_read_community (agent_rd_str); // this agent's read community string

  //  1. verify we have a valid read string else drop pdu (no response to caller)
  if (mgr_rd_str != agent_rd_str)
    {
     ACE_DEBUG ((LM_DEBUG, "agent_impl::handle_get: invalid read community recvd\n"));
     return 0;
    }

  // 2. iterate over each varbind in the pdu, filling providing responses
  bool done {};
  for (int i = 0; i < pdu.get_vb_count () && !done; ++i)
    {
      Vb vb {};
      pdu.get_vb (vb, i);
      if (get_response (vb))
        {
          set_error_status (&pdu, SNMP_ERROR_NO_SUCH_NAME);
          set_error_index (&pdu, i);
          done = true;
        }
      else
        pdu.set_vb (vb, i);
    }

  // 3. lastly, return the pkt to the caller
  return respond (pdu, target);
}

// this routine makes up the brains of the agent
// it knows only the MIB II system group set of variables for a get operation
int agent_impl::get_response (Vb& vb)
{
  // these objects represent the MIB II system group per RFC 1213
   static Oid const sysDescr ("1.3.6.1.2.1.1.1.0"),
     sysObjectID ("1.3.6.1.2.1.1.2.0"), sysUpTime ("1.3.6.1.2.1.1.3.0"),
     sysContact ("1.3.6.1.2.1.1.4.0"), sysName ("1.3.6.1.2.1.1.5.0"),
     sysLocation ("1.3.6.1.2.1.1.6.0"), sysServices ("1.3.6.1.2.1.1.7.0");

  Oid oid {};
  vb.get_oid (oid);
  if (oid == sysDescr) {
    OctetStr const desc ("ASNMP Prototype Agent 1.0");
    vb.set_value (desc);
  }
  else if (oid == sysObjectID) { // the IANA gives assigns Enterprise Numbers
    // see ftp://ftp.isi.edu/in-notes/iana/assignments/enterprise-numbers
    // for the official list of enterprise numbers. Then under this tree
    // assign a unique subtree to identify this agent
    Oid const id ("1.3.6.1.4.1.2533.9.1");
    vb.set_value (id);
  }
  else if (oid == sysUpTime) {
    ACE_Time_Value tv {};
    agent_clock_.elapsed_time (tv);
    TimeTicks tt (tv.msec ());
    vb.set_value (tt);
  }
  else if (oid == sysContact) {
    OctetStr const contact ("mrm@acm.org");
    vb.set_value (contact);
  }
  else if (oid == sysName) {
    OctetStr const fqdn ("foo.org"); // extract this from the gethostbyname() TODO
    vb.set_value (fqdn);
  }
  else if (oid == sysLocation) {
    OctetStr const loc ("");
    vb.set_value (loc);
  }
  else if (oid == sysServices) {
    SnmpInt32 const svcs (72);
    vb.set_value (svcs);
  }
  else
    return 1; // noSuchName

  return 0;
}

int agent_impl::handle_get_next (Pdu &, UdpTarget &)
{
   ACE_TRACE ("agent_impl::handle_get_next -NI");
  return 0;
}

int agent_impl::handle_set (Pdu &, UdpTarget &)
{
 ACE_TRACE ("agent_impl::handle_set -NI");
 return 0;
}

// stuff used by process_requests

// called when SIGINT
static sig_atomic_t finished {};

extern "C" void
sig_handler (int)
{
  ACE_TRACE ("::sig_handler");
  finished = 1;
}

int agent_impl::process_requests ()
{
  ACE_TRACE ("agent_impl::process_requests");
  ACE_Reactor reactor;

  ACE_Sig_Action sa (sig_handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  // Read data from other side.
  if (reactor.register_handler (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_Reactor::register_handler"), -1);

  // TODO: register signal handler to shut down gracefully here

  while (!finished)
    {
      reactor.handle_events ();
      ACE_DEBUG ((LM_DEBUG, "return from handle events\n"));
    }

  ACE_DEBUG ((LM_DEBUG, "return from handle events - normal shut down\n"));
  return 0;
}
