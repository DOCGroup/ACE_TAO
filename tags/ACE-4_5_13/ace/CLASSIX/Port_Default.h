/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ipp_ace
// 
// = FILENAME
//    CLASSIX/Port_Default.h
//
// = AUTHOR
//    Wei Chiang
//
// = COPYRIGHT
//     Copyright 1998 Nokia Telecommunications
// 
// ============================================================================

#if !defined (ACE_CLASSIX_PORT_DEFAULT_H)
#define ACE_CLASSIX_PORT_DEFAULT_H

#include "ace/Singleton.h"
#include "CLASSIX/Port.h"

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
#include "CLASSIX/Port_Default.i"
#endif /* __ACE_INLINE__ */

/* ------------------------------------------------------------------------- */
typedef ACE_Singleton<ACE_CLASSIX_Port_Default, ACE_SYNCH_NULL_MUTEX> 
        ACE_CLASSIX_DEFAULT_PORT;
/* ------------------------------------------------------------------------- */

#endif /* ACE_CLASSIX_PORT_DEFAULT_H */
