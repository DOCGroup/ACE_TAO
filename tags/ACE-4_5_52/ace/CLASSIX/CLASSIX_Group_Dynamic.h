/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
// 
// = FILENAME
//     CLASSIX_Group_Dynamic.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
// 
// ============================================================================

#ifndef ACE_CLASSIX_GROUP_DYNAMIC_H
#define ACE_CLASSIX_GROUP_DYNAMIC_H

/* ------------------------------------------------------------------------- */
#include "ace/CLASSIX/CLASSIX_Group.h"

class ACE_CLASSIX_Group_Dynamic : public ACE_CLASSIX_Group
    // = TITLE
    //   Wrapper over Chorus's Group which is created dynamically and not named
    //   by a <Stamp>.
    //   
    // = DESCRIPTION
    //   This is a subclass of <ACE_CLASSIX_Group>.
    //   
    //   Its encapsulated group is created dynamically and the
    //   <KnCap> is the name as well as the unique identifier.
    //   
    // = SEE ALSO
    //   <ACE_CLASSIX_Group>, <ACE_CLASSIX_Group_Stamp>
{
  public:
 
  ACE_CLASSIX_Group_Dynamic(void);
  // default constructor... create a group use Chorus IPC's K_DYNAMIC option

    virtual ACE_CLASSIX_Addr::Addr_Type is_type(void) const;
    // returns Dynamic type

  private:
 
  ACE_CLASSIX_Group_Dynamic(ACE_CLASSIX_Group_Dynamic const&);
  ACE_CLASSIX_Group_Dynamic const& operator=(ACE_CLASSIX_Group_Dynamic const&);
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Group_Dynamic.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_CLASSIX_GROUP_DYNAMIC_H */
