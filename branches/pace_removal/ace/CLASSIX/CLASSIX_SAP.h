/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     CLASSIX_SAP.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================

#ifndef ACE_CLASSIX_SAP_H
#include "ace/pre.h"
#define ACE_CLASSIX_SAP_H

/* ------------------------------------------------------------------------- */
#include "ace/CLASSIX/CLASSIX_Port.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_CLASSIX_SAP
    // = TITLE
    //   Class implementing the SAP over a Chorus port.
    //
    // = DESCRIPTION
    //   A SAP has a port which is referred to as the local port.
    //   This class provides methods to manipulate this port.
    //
    //   The SAP will always put the port in DISABLED state on exit.
    //
    //   In CLASSIX, a local SAP is always a port, a peer SAP can be
    //   a port or a group of port.  This class is for local SAP and
    //   <ACE_Peer_SAP> is for peer SAP.
    //
    // = SEE ALSO
    //   <ACE_Peer_SAP>
    //
{
 public:
    enum
    {
        DEFAULT_PRIORITY = 10
    };

    // = INITIALIZATION
    ACE_CLASSIX_SAP(void);
    // Creates a SAP, using the address of the actor's default port.
    ACE_CLASSIX_SAP(const ACE_Addr&);
    // Creates a SAP, using the specified address as the local address.
     virtual ~ACE_CLASSIX_SAP(void);

    // = ACCESS
    void set_addr(const ACE_Addr&);
    void set_addr(const ACE_CLASSIX_Port_Core&);
    // Sets the <local_addr_> based on the given address information.
    // The caller should make sure that the original address can be removed!
    int  set(const ACE_Addr&);
    // Returns 0, if successful; returns -1, otherwise
    // same as set_addr(const ACE_Addr&);
    int  set(const ACE_CLASSIX_Port_Core&);
    // Returns 0, if successful; returns -1, otherwise
    // same as void set_addr(const ACE_CLASSIX_Port_Core&);
    int set (const KnUniqueId&);
    // Returns 0, if successful; returns -1, otherwise

    void set_handle (ACE_HANDLE /* port_no */);
    // Sets the port address according to <port_no>

    int  get_addr(ACE_Addr& /* return_addr */) const;
    // Returns address information to the supplied location.
    // if successful, return 0 and the return_addr parameter contains the
    // address info.
    // Otherwise, return -1
    const ACE_CLASSIX_Port& get_addr(void) const;
    // Returns a reference to the address information
    ACE_HANDLE get_handle(void) const;
    // Get handles that the Reactor uses
    virtual int is_configured(void) const;
    // Returns 1, if address information is proper; Returns 0 otherwise
    int is_selectable(void) const;
    // Retruns 1, if the local SAP is enabled.

    // = CONTROL
    int selectable(int = ACE_CLASSIX_SAP::DEFAULT_PRIORITY /* priority */);
    // Puts the SAP in the ENABLED state so that data the port associated
    // with its local addess can be monitored by the CLASSIX's Reactor.

    int unselectable(int = 0 /* not used */);
    // Makes the SAP DISABLED and therefore the SAP can not be monitored
    // by the CLASSIX's Reactor..
    int open(const ACE_Addr&);
    int open(const ACE_CLASSIX_Port_Core*);
    // Sets the address information according to the supplied port
    // Returns 0 on success; returns -1 otherwise
    int close(void);
    // removes the port information
    // Returns 0 on success (always); returns -1 otherwise

    void clear(void);
    // erase local port info

#if 0
    int control(u_int = K_BROADMODE);
    int control(u_int, /* mode */ int /* site */ );
    // Interface for CLASSIX' send mode
#endif


    // = HELPER
    void dump(void) const;
    // dump the state of an object

    ACE_ALLOC_HOOK_DECLARE;
    // declare the dynamic allocation hooks

 protected:
#if 0
    virtual set_mode_(u_int = K_BROADMODE); /* theMode */
    // Sets the addressing mode with the current setting of coTarget
    virtual set_mode_(u_int, /* mode */ int /* site */);
    // Sets the addressing mode as well as the coTarget
#endif
    // Disable copy constructor/assignment
    ACE_CLASSIX_SAP(ACE_CLASSIX_SAP const&);
    ACE_CLASSIX_SAP const& operator=(ACE_CLASSIX_SAP const&);

    ACE_CLASSIX_Port   local_addr_;
    int                enabled_; // 1 if enabled, 0 if disabled (Default)
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_SAP.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_CLASSIX_SAP_H */
