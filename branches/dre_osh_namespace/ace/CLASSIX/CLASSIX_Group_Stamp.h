/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     CLASSIX_Group_Stamp.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================

#ifndef ACE_CLASSIX_GROUP_STAMP_H
#include /**/ "ace/pre.h"
#define ACE_CLASSIX_GROUP_STAMP_H

/* ------------------------------------------------------------------------- */
#include "ace/CLASSIX/CLASSIX_Group.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_CLASSIX_Group_Stamp : public ACE_CLASSIX_Group
    // = TITLE
    //   Wrapper over Chorus's Group using <Stamp> for naming the group.
    //
    // = DESCRIPTION
    //   This is a subclass of <ACE_CLASSIX_Group>.
    //
    //   Its encapsulated group is named by a <stamp>.  However,
    //   <KnCap> is still the unique identifier of the group.
    //
    // = NOTE
    //   As at the time of writing, The stamp is contained in the ui.uiTail
    //   field of <KnCap>.
    //
    // = SEE ALSO
    //   <ACE_CLASSIX_Group>, <ACE_CLASSIX_Group_Dynamic>
    //
{
 public:
    enum
    {
        ILLEGAL_STAMP = 0
        /* Have we defined the range of valid stamps? */
    };

    // = INITIALIZATION
    ACE_CLASSIX_Group_Stamp();
    // default constructor.  Does not contain group information..
    ACE_CLASSIX_Group_Stamp(int /* stamp */);
    // creates a group use the specfied stamp
    virtual void set_addr(void*,  /* pointer to the stamp */
                          int = 0 /* not used */);
    // Replaces the existing group according to the supplied stamp

    // = ACCESS
    virtual ACE_CLASSIX_Addr::Addr_Type is_type(void) const;
    // returns STAMP type

    int get_stamp() const;
    // Returns the stamp
    // If the address is not configured, the return value is unpredictable.

    // = HELPER
    void dump(void) const;



 private:
    u_int stamp_;

    // disable copy/assignment constructor
    ACE_CLASSIX_Group_Stamp(ACE_CLASSIX_Group_Stamp const&);
    ACE_CLASSIX_Group_Stamp const& operator=(ACE_CLASSIX_Group_Stamp const&);
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Group_Stamp.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_CLASSIX_GROUP_STAMP_H */
