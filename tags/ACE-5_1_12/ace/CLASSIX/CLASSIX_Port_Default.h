/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    CLASSIX_Port_Default.h
//
// = AUTHOR(S)
//    Nokia Telecommunications
//
// ============================================================================

#ifndef  ACE_CLASSIX_PORT_DEFAULT_H
#include "ace/pre.h"
#define ACE_CLASSIX_PORT_DEFAULT_H

#include "ace/Singleton.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CLASSIX/CLASSIX_Port.h"

class ACE_CLASSIX_Port_Default : public ACE_CLASSIX_Port
{
    // = TITLE
    //    Implements the address wrapper of an actor's default port.
    //
    // = DESCRIPTION
    //   This corresponds to the address of an actor's default port
    //   which does not change
    //   in the life time of the actor.  Therefore the address information
    //   in this class can not be modified once it is created.
    //
    //   Each actor has one and only one default port.  Therefore an instance
    //   of this class is a singleton.  The protected constructor and the
    //   friendship with the ACE_Singleton preventys this class from being
    //   instantiated by other classes.
    //
    // = NOTE
    //
    // = SEE ALSO
    //   <ACE_CLASSIX_PORT>
    //
    friend class ACE_Singleton<ACE_CLASSIX_Port_Default, ACE_SYNCH_NULL_MUTEX>;

    /* -----------------------------------------------------*/
    // = Access
    virtual void set_addr(void*, int);
    ACE_CLASSIX_Port const&  operator =(ACE_CLASSIX_Port const&);
    ACE_CLASSIX_Port const&  operator =(ACE_Addr const&);
    // Does nothing...The default port's address cannot be changed

    ACE_ALLOC_HOOK_DECLARE;
    // Declare the dynamic allocation hooks.

protected:
    /* -----------------------------------------------------*/
    // = Initialization methods.
    ACE_CLASSIX_Port_Default ();
    // Wrapper for the Actor's default port
};


#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Port_Default.i"
#endif /* __ACE_INLINE__ */

/* ------------------------------------------------------------------------- */
typedef ACE_Singleton<ACE_CLASSIX_Port_Default, ACE_SYNCH_NULL_MUTEX>
        ACE_CLASSIX_DEFAULT_PORT;
/* ------------------------------------------------------------------------- */

#include "ace/post.h"
#endif /* ACE_CLASSIX_PORT_DEFAULT_H */
