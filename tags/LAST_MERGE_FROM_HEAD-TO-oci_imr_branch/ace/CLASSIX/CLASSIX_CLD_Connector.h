/* -*- C++ -*- */
// $Id$

/*
// ============================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     CLASSIX_CLD_Connector.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================
*/

#ifndef ACE_CLASSIX_CLD_CONNECTOR_H
#include /**/ "ace/pre.h"
#define ACE_CLASSIX_CLD_CONNECTOR_H

#include "ace/CLASSIX/CLASSIX_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/* ------------------------------------------------------------------------- */

class ACE_CLASSIX_CLD_Connector
    // = TITLE
    //     Defines an CLASSIX IPC connectionless connection factory for the
    //     template methods in <ACE_Connector>.
    //
    // = DESCRIPTION
    //     This is used when a client needs to communicate with a server
    //     whose SAP address is known beforehand.
    //
    //     Although there is a connection
    //     procedure, no connection is actually taken place
    //     between the client and server, since Chorus IPC does not have
    //     connection semantics.
    //     The connect procedure is necessary for
    //     compatibility with <ACE_SOCK_Connector> class.
    //
    //     This class will simplify the client's
    //     program, since it provides connection-like communication
    //     mechanism.
    //
    // = NOTES
    //     The server in this case trusts everyone who lands on the
    //     server's SAP.
    //
    // = SEE ALSO
    //   ACE_CLASSIX_COD_Connector
{
  public:
    // =  Connect options
    enum OPTIONS
    {
        // Do not enable the local SAP
        // (so that the Reactor will not monitor the port)
        ENABLE  = 0x1
    };

    // = Initialization routines.
    ACE_CLASSIX_CLD_Connector (void);
    // Default constructor.

    ACE_CLASSIX_CLD_Connector (ACE_CLASSIX_Stream &new_stream,
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

    virtual int connect (ACE_CLASSIX_Stream &new_stream,
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

    virtual int reset_new_handle (ACE_HANDLE handle);
    // Factory method used in ACE_Connector::handle_output().
    // Meant for resetting any event associations on this handle
    // Does nothig.

    // = HELPER
    void dump(void) const;

 private:

    ACE_CLASSIX_CLD_Connector(ACE_CLASSIX_CLD_Connector const&);
    ACE_CLASSIX_CLD_Connector const& operator=(ACE_CLASSIX_CLD_Connector const&);
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_CLD_Connector.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_CLASSIX_CLD_CONNECTOR_H */
