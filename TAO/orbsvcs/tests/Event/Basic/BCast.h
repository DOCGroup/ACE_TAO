/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   BCast
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_BCAST_H
#define EC_BCAST_H

#include "Driver.h"
#include "orbsvcs/RtecUDPAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class EC_BCast : public EC_Driver
{
  //
  // = TITLE
  //   Test the EC bcast
  //
  // = DESCRIPTION
  //
public:
  EC_BCast (void);
  // Constructor

  // = The EC_Driver methods
  virtual int parse_args (int& argc, char* argv[]);
  virtual void print_args (void) const;
  virtual void print_usage (void);
  // add some command line args to enable/disable bcastions

  virtual void modify_attributes (TAO_EC_Event_Channel_Attributes& attr);
  // set the bcastion flags

  void execute_test (TAO_ENV_SINGLE_ARG_DECL);
  // Don't run the suppliers, just test connect and disconnect calls.

  void dump_results (void);
  // Don't dump the EC_Driver results, they are meaningless.

private:
  const char* bcast_address_;
  // The IP address used to broadcast

  u_short bcast_port_;
  // The port used to send and receive bcast messages...
};

// ****************************************************************

class Simple_Address_Server : public POA_RtecUDPAdmin::AddrServer
{
  // = DESCRIPTION
  //   A fixed address server
public:
  Simple_Address_Server (const ACE_INET_Addr& address);
  // constructo

  virtual void get_addr (const RtecEventComm::EventHeader& header,
                         RtecUDPAdmin::UDP_Addr& address
                         TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  RtecUDPAdmin::UDP_Addr address_;
  // The UDP addres...
};

#endif /* EC_BCAST_H */
