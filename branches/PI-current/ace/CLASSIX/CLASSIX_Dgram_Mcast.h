/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     CLASSIX_Dgram_Mcast.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================

#ifndef ACE_CLASSIX_DGRAM_MCAST_H
#define ACE_CLASSIX_DGRAM_MCAST_H

/* ------------------------------------------------------------------------- */
#include <ace/CLASSIX/CLASSIX_Stream.h>

class ACE_CLASSIX_Dgram_Mcast : public ACE_CLASSIX_Stream
    // = TITLE
    //   Provides Dgram_Mcast interfaces for CLASSIX IPC communications.
    //
    // = DESCRIPTION
    //   This class represents multiple end-points for message delivery.
    //   All the remote ports are belong to the same group identified by
    //   a stamp.
    //   It uses the broadcast mode provided by ipcSend() to deliver
    //   messages.
    //
    //   This class is most likely used in the server.
    //
{
 public:
    // = INITIALIZATION
    ACE_CLASSIX_Dgram_Mcast(void);
    // Default constructor, does not setup anything

    ACE_CLASSIX_Dgram_Mcast(int /* stamp */,
                            const ACE_CLASSIX_Port& =
                            *ACE_CLASSIX_DEFAULT_PORT::instance());
    // remote SAP = port group, local SAP = default port

    int set_saps(int /* stamp */,
             const ACE_CLASSIX_Port& =
             *ACE_CLASSIX_DEFAULT_PORT::instance());
    // remote SAP = port group, local SAP = default port

    //virtual ssize_t send (const void *, /* buf */
    // size_t /* n */) const;


    // = HELPER
    void dump (void) const;
    // Dump the state of an object.

    ACE_ALLOC_HOOK_DECLARE;
    // Declare the dynamic allocation hooks.

 protected:

    virtual int set_mode_(u_int = K_BROADMODE); /* theMode */
    // Sets the addressing mode with the current setting of coTarget
    virtual int set_mode_(u_int, /* mode */ int /* site */);
    // Sets the addressing mode as well as the coTarget

    // Site information for delivery mode, see <ACE_CLASSIX_Stream::dest_ >
    // Can be changed by control()/set_mode_()
    int                 dest_site_;


 private:
   // Disable copy constructor
  ACE_CLASSIX_Dgram_Mcast(ACE_CLASSIX_Dgram_Mcast const&);
  ACE_CLASSIX_Dgram_Mcast const& operator=(ACE_CLASSIX_Dgram_Mcast const&);
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Dgram_Mcast.i"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#endif /* __ACE_INLINE__ */

#endif /* ACE_CLASSIX_DGRAM_MCAST_H */
