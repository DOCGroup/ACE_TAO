// -*- C++ -*-

//=============================================================================
/**
 *  @file   SCTP_Fault_Detector.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef SCTP_FAULT_DETECTOR_H
#define SCTP_FAULT_DETECTOR_H

#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/SOCK_SEQPACK_Association.h"
#include "ace/SOCK_SEQPACK_Acceptor.h"
#include "ace/SOCK_SEQPACK_Connector.h"
#include "ConnectionHandler_T.h"
#include "Fault_Detector_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// make sure that the code compiles cleanly even if SCTP is not
// available. If SCTP is not installed, program will exit early in
// main() with an error message
#ifdef ACE_HAS_SCTP
extern "C" {
#include <netinet/sctp.h>
};
#else
#ifndef IPPROTO_SCTP
#define IPPROTO_SCTP 132
#endif // !IPPROTO_SCTP
#define SCTP_NODELAY 1
#endif // ACE_HAS_SCTP


typedef ACE_Acceptor<ConnectionAcceptHandler<ACE_SOCK_SEQPACK_ASSOCIATION>, ACE_SOCK_SEQPACK_ACCEPTOR>
                      SCTP_ConnectionAcceptHandler;
typedef ConnectionDetectHandler<ACE_SOCK_SEQPACK_ASSOCIATION> STCP_ConnectionDetectHandler;

class SCTP_Connector : public ACE_SOCK_SEQPACK_Connector
{
public:
  SCTP_Connector();
  int connect (ACE_SOCK_SEQPACK_Association &new_association,
               const ACE_Addr &remote_sap,
               const ACE_Time_Value *timeout = 0,
               const ACE_Addr &local_sap = ACE_Addr::sap_any,
               int reuse_addr = 0,
               int flags = 0,
               int perms = 0,
               int protocol = 132);
  void set_heart_beat(int hb);
private:
  int heart_beat_;
};

class STCP_Fault_Detector : public Fault_Detector_T<
          SCTP_ConnectionAcceptHandler,
          SCTP_Connector,
          STCP_ConnectionDetectHandler >
{
public:
    virtual ~STCP_Fault_Detector();
private:
    virtual int parse_conf(int argc, char** argv);
};

#endif
