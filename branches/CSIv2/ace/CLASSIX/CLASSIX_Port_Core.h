/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     CLASSIX_Port_Core.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================

#ifndef ACE_CLASSIX_PORT_CORE_H
#include /**/ "ace/pre.h"
#define ACE_CLASSIX_PORT_CORE_H

/* ------------------------------------------------------------------------- */
#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <ipc/chIpc.h>


/* ------------------------------------------------------------------------- */
class ACE_Export ACE_CLASSIX_Port_Core
{
    // = TITLE
    //    Defines interfaces for allocation/deallocation of a Chorus
    //    IPC port
    //
    // = DESCRIPTION
    //   This class allocates a port in the constructor and deallocates it in
    //   the destructor.

public:
    /* -----------------------------------------------------*/
    // = Initialization methods.
    ACE_CLASSIX_Port_Core (void);
    // Allocates a port from the OS
    //
    virtual ~ACE_CLASSIX_Port_Core (void);
    // Deallocates the port from the OS.

    virtual const KnUniqueId& get_id (void) const;
    // Returns a reference to the port id.
    virtual ACE_HANDLE get_handle (void) const;
    // Returns the port no(used as an ACE_HANDLE)
    virtual void* get_addr(void) const;
    // Returns a pointer to the location containing the raw
    // address of the port.
    int is_configured(void) const;
    // Returns 1, if the port has been successfully created.

#if 0
    int enable(int /* priority */) const;
    int disable (int) const;
    // Puts/Removes the port into/from the set of monitored ports.
#endif
    /* -----------------------------------------------------*/
    // = Helper
    //
    void dump (void) const;
    // Dump the state of an object.


    ACE_ALLOC_HOOK_DECLARE;
    // Declare the dynamic allocation hooks.

    struct Addr
    {
        KnUniqueId                  id;    // port address: unique identifier
        ACE_HANDLE                  handle;// port no: local identifier.

    };

protected:
    void clear_config_();
    // Indicates that port has been allocated.
    void set_config_();

    ACE_CLASSIX_Port_Core::Addr addr_;
    int                         config_; // Sets to 1, if configured;
                                         // 0 otherwise
private:
    // Disallow copy constructor/assignment
    ACE_CLASSIX_Port_Core(ACE_CLASSIX_Port_Core const&);
    ACE_CLASSIX_Port_Core const&  operator =(ACE_CLASSIX_Port_Core const&);
};
/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Port_Core.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_CLASSIX_PORT_CORE_H */
