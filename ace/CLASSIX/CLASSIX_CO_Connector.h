/* -*- C++ -*- */
// $Id$

/*
// ============================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     CLASSIX_CO_Connector.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================
*/

#ifndef ACE_CLASSIX_CO_CONNECTOR_H
#include /**/ "ace/pre.h"
#define ACE_CLASSIX_CO_CONNECTOR_H

#include "ace/CLASSIX/CLASSIX_CLD_Connector.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* ------------------------------------------------------------------------- */

class ACE_CLASSIX_CO_Connector : public ACE_CLASSIX_CLD_Connector
    // = TITLE
    //     Defines an CLASSIX IPC connection factory for the
    //     template methods in <ACE_Connector>.
    //
    // = DESCRIPTION
    //     This is used when a client needs to communicate with a server
    //     whose SAP address is unknown.
    //
    //     <ACE_CLASSIX_Connect> and <ACE_CLASSIX_Acceptor> exchanges
    //     address(SAP) information to establish a logical link.
    //
    // = NOTES
    //      There is no other identifier and sequence information
    //      passed between the acceptor and the connector.
    //      Some applications may need to have a
    //      reliable protocol for the connection setup procedure.
    //
    // = SEE ALSO
    //   ACE_CLASSIX_CLD_Connector
    //   ACE_CLASSIX_Acceptor
    //
{
  public:
    // = Initialization routines.
    ACE_CLASSIX_CO_Connector (void);
    // Default constructor.

    ACE_CLASSIX_CO_Connector (ACE_CLASSIX_Stream &new_stream,
                               const ACE_Addr &remote_sap
                               = ACE_Addr::sap_any,
                               ACE_Time_Value *timeout = 0,
                               const ACE_Addr &local_sap = ACE_Addr::sap_any,
                               int reuse_addr = 0,
                               int flags = 0,
                               int perms = 0,
                               int protocol_family
                               = ACE_CLASSIX_Addr::AF_CLASSIX,
                               int protocol = 0);
    // Emulates connection semantics so that it can be plugged into
    // <ACE_Connector>
    // Actively connect local SAP and remote SAP togeter.
    // Produce a <new_stream> if things go well.
    //
    // <remote_sap> is the peer's address. In ACE_SOCK_Connector it refers to
    // the acceptor's address.  In connectionless mode, we do not have
    // an acceptor.  The <remote_sap> address will be copied to the new stream
    // if things goes well.
    //
    // If <local_sap> refers to any address, a port will be
    // allocated and will be used as a local SAP.
    //
    // The rest parameters are there so that this class can be plugged into
    // <ACE_Connector>.

    int connect (ACE_CLASSIX_Stream &new_stream,
                 const ACE_Addr &remote_sap = ACE_Addr::sap_any,
                 ACE_Time_Value *timeout = 0,
                 const ACE_Addr &local_sap = ACE_Addr::sap_any,
                 int reuse_addr = 0,
                 int flags = 0,
                 int perms = 0,
                 int protocol_family = ACE_CLASSIX_Addr::AF_CLASSIX,
                 int protocol = 0);

    // Factory method used in <ACE_Connector>::connect_svc_handler()

    // Initialization method when default constructor is used.
    // See the comments for the corresponding constructor

    // = HELPER
    void dump(void) const;


 private:

    ACE_CLASSIX_CO_Connector(ACE_CLASSIX_CO_Connector const&);
    ACE_CLASSIX_CO_Connector const& operator=(ACE_CLASSIX_CO_Connector const&);
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_CO_Connector.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_CLASSIX_CO_CONNECTOR_H */
