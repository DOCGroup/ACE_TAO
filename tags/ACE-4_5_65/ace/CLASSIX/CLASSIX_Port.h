/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    CLASSIX_Port.h
//
// = AUTHOR
//    Nokia Telecommunications
//
// ============================================================================

#ifndef ACE_CLASSIX_PORT_H
#define ACE_CLASSIX_PORT_H

#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CLASSIX/CLASSIX_OS.h"
#include "ace/CLASSIX/CLASSIX_Addr.h"


class ACE_Export ACE_CLASSIX_Port : public ACE_CLASSIX_Addr
{
    // = TITLE
    //    Defines the ClassiX IPC Port address format for "ACE Socket" like
    //    interfaces
    //
    // = DESCRIPTION
    //   The port address contains two fields:
    //   a global unique identifier  and a local identifier.
    //   The global unique identifier is also referred to as the port address
    //   and the local identifier as the port no.
    //
    // = NOTE
    //   In Chorus, one can always convert from a port unique identifier to
    //   a port no and vice versa.  This class is implemented based on this
    //   premise.
    //
    // = SEE ALSO
    //   <ACE_CLASSIX_PORT_Default>, <ACE_CLASSIX_Port_Core>
    //
public:
    /* -----------------------------------------------------*/
    // = Initialization methods.
    ACE_CLASSIX_Port ();
    // Default constructor.
    // The address corresponds to the default port of the actor.

    ACE_CLASSIX_Port (const ACE_CLASSIX_Port &);
    // Copy constructor.
    ACE_CLASSIX_Port (const ACE_Addr &);
    // Copy constructor.

    ACE_CLASSIX_Port (const ACE_CLASSIX_Port_Core&);
    // Creates an <ACE_CLASSIX_Port> from <ACE_CLASSIX_Port_Core>

    ACE_CLASSIX_Port (const KnUniqueId& /* port_id */);
    // Creates an <ACE_CLASSIX_Port> from the given <port_id>

    ACE_CLASSIX_Port (ACE_HANDLE /* port_no */);
    // Creates an <ACE_CLASSIX_Port> from the given <port_no>

    ACE_CLASSIX_Port (void* /* location */, int /* length */);
    // Create an <ACE_CLASSIX_Port> from the address in raw format.

    ACE_CLASSIX_Port const&  operator =(ACE_Addr const&);


    /* -----------------------------------------------------*/
    // = ACCESS

    virtual int addr_to_string (ASYS_TCHAR addr[], size_t) const;
    // Transform the current <ACE_CLASSIX_Port> address into string format,
    // which is in the form "actor-id:port-number"

    int set (const KnUniqueId& /* port_id */);
    // Sets the <ACE_CLASSIX_Port_Basic> from a <port_id>

    virtual void set_addr (void * /* addr location */,
                           int /* len */);
    // Set the address as the one pointed to by the location pointer.
    // The address contains <len> bytes.
    // Would prefer to return the status, but the base class uses void.
    int set_handle (ACE_HANDLE /* port_no */);
    // Sets the <addr_> field from a <port_no>

    virtual void *get_addr (void) const;
    // Returns a pointer to the address:,
    // <ACE_CLASSIX_Port_Core::ipp_port_addr>
    virtual const KnUniqueId& get_id (void) const;
    // Returns a reference to the port id.
    virtual ACE_HANDLE get_handle (void) const;
    // Returns the port no.
    int is_configured(void) const;
    // Returns 1, if address information is proper; Returns 0 otherwise
    virtual ACE_CLASSIX_Addr::Addr_Type is_type(void) const;
    // returns PORT type

    /* -----------------------------------------------------*/
    // = Control
    //
    virtual int enable(int   /* receive priority */) const;
    // Puts the port into the set of monitored ports.
    virtual int disable(void) const;
    // Removes the port from the set of monitored ports.
    virtual void clear(void);
    // Remove port information

    /* -----------------------------------------------------*/
    // = Comparison
    //
    int operator == (const ACE_CLASSIX_Port &) const;
    // Compare two addresses for equality.  The addresses are considered
    // equal if they have the same content in the KnUniqueId address structure.

    int operator != (const ACE_CLASSIX_Port &) const;
    // Compare two addresses for inequality.

    /* -----------------------------------------------------*/
    // = Helper
    //
    void dump (void) const;
    // Dump the state of an object.


    ACE_ALLOC_HOOK_DECLARE;
    // Declare the dynamic allocation hooks.

private:
    /* -----------------------------------------------------*/
    // = Direct initialization methods (useful after the object has been
    // constructed).
    // Will create/reset the port only if the port no was <ACE_CLASSIX_ANY>
    // Returns 0 on success, -1 otherwise.
    //

    int set_ (const ACE_Addr &);
    // Sets the <ACE_CLASSIX_Port_Basic> from another <ACE_CLASSIX_Port_Basic>.



private:
    ACE_CLASSIX_Port_Core::Addr           addr_;
};

/* ------------------------------------------------------------------------- */
#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Port.i"
#endif /* __ACE_INLINE__ */


#endif /* ACE_CLASSIX_PORT_H */
