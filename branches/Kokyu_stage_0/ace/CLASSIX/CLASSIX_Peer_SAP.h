/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     CLASSIX_Peer_SAP.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================

#ifndef ACE_CLASSIX_PEER_SAP_H
#include "ace/pre.h"
#define ACE_CLASSIX_PEER_SAP_H

/* ------------------------------------------------------------------------- */
#include <ace/CLASSIX/CLASSIX_SAP.h>
#include <ace/CLASSIX/CLASSIX_Port_Default.h>

class ACE_CLASSIX_Peer_SAP
    // = TITLE
    //   Class implementing the peer SAP.
    //
    // = DESCRIPTION
    //
    //   In CLASSIX, a local SAP is always a port, a peer SAP can be
    //   a port or a group of port.  This class is for peer SAP and
    //   <ACE_SAP> is for local SAP.
    //
    // = SEE ALSO
    //   <ACE_SAP>
{
 public:
    // = INITIALIZATION
    ACE_CLASSIX_Peer_SAP(void);
    // Default constructor.
    // Needs to be initialized by a set() operation.
    ACE_CLASSIX_Peer_SAP(const ACE_CLASSIX_Port_Core&);/* peer port */

    ACE_CLASSIX_Peer_SAP(const ACE_CLASSIX_Port&);/* peer port address */
    ACE_CLASSIX_Peer_SAP(const ACE_Addr&);        /* peer port address */

    ACE_CLASSIX_Peer_SAP(const KnUniqueId&);      /* peer port address */
    ACE_CLASSIX_Peer_SAP(const KnCap&);           /* peer group address */
    ACE_CLASSIX_Peer_SAP(int /* stamp */);       /* stamp for peer group */

    ~ACE_CLASSIX_Peer_SAP(void);

    int set(const ACE_CLASSIX_Port_Core&); /* peer port */

    int set(const ACE_Addr&);              /* peer port address */

    int set(const KnUniqueId&);            /* peer port address */
    int set(const KnCap&);                 /* peer group address */
    int set(int /* stamp */);   /* stamp for the peer group */

    // = ACCESS
    const ACE_CLASSIX_Addr* get_addr(void) const;
    // Returns peer's address, either a port or a group of ports
    int is_configured(void) const;
    // Returns 1, if address information is proper; Returns 0 otherwise

    // = CONTROL
#if 0
    int control(u_int = K_BROADMODE);
    int control(u_int, /* mode */ int /* site */ );
    // Interface for CLASSIX' send mode
#endif
    // = HELPER
    void dump (void) const;
    // Dump the state of an object.

    ACE_ALLOC_HOOK_DECLARE;
    // Declare the dynamic allocation hooks.

 protected:
#if 0
    virtual set_mode_(u_int = K_BROADMODE); /* theMode */
    // Sets the addressing mode with the current setting of coTarget
    virtual set_mode_(u_int, /* mode */ int /* site */);
    // Sets the addressing mode as well as the coTarget
#endif
    ACE_CLASSIX_Addr     *peer_addr_;

 private:
    // Disable copy constructor
    ACE_CLASSIX_Peer_SAP(ACE_CLASSIX_Peer_SAP const&);
    ACE_CLASSIX_Peer_SAP const& operator=(ACE_CLASSIX_Peer_SAP const&);
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Peer_SAP.i"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_CLASSIX_PEER_SAP_H */
