/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//     ace
//
// = FILENAME
//     CLASSIX/Group.h
//
// = AUTHOR(S)
//     Nokia Telecommunications
//
// ============================================================================

#ifndef ACE_CLASSIX_GROUP_H
#include "ace/pre.h"
#define ACE_CLASSIX_GROUP_H

/* ------------------------------------------------------------------------- */
#include "ace/CLASSIX/CLASSIX_Addr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CLASSIX/CLASSIX_Port.h"

class ACE_CLASSIX_Group : public ACE_CLASSIX_Addr
    // = TITLE
    //   Wrapper over Chorus's Group concept.
    //
    // = DESCRIPTION
    //   Provides interface to insert and remove ports in the group which is
    //   created based on a given KnCap. This"<KnCap> is the unique
    //   identifier of the group.
    //
    // = NOTE
    //   Chorus does not provide facility to remove a group from the system,
    //   nor explicitly create a group.
    //   As such, the wrapper allows users to freely change from one group
    //   to the other within the same <ACE_CLASSIX_Group> instance.
    //
    // = SEE ALSO
    //   <ACE_CLASSIX_Group_Stamp>, <ACE_CLASSIX_Group_Dynamic>
    //
{
 public:
    // = INITIALIZATION
    ACE_CLASSIX_Group(void*, /* group id */ int /* len */);
    // Sets up the group wrapper using the supplied group id.
    ACE_CLASSIX_Group(const KnCap&);
    // Sets up the group wrapper using the supplied KnCap

    // = ACCESS
    int is_configured(void) const;
    // Returns 1, if it contains valid group id; 0, otherwise.
    virtual ACE_CLASSIX_Addr::Addr_Type is_type(void) const;
    // returns GROUP type
    void* get_addr(void) const;
    // Returns the group id
    virtual const KnUniqueId& get_id (void) const;
    // Returns a reference to the unique identifier
    virtual void set_addr(void* /* source */, int /* len */);
    // Sets the group information  according to the supplied data
    // in raw format.
    virtual void set_addr(const KnCap&);
    // Sets the group information  according to the supplied KnCap.
    const KnCap& get_cap(void) const;
    // Returns the capability to the supplied location
    // If the address is not configured, the return value is unpredictable.
    int insert(const ACE_CLASSIX_Port*);
    int remove (const ACE_CLASSIX_Port*);
    // Inserts/removes the specified port in to the group
    // Returns 0, if successful; returns a negative value otherwise

    int operator == (const ACE_CLASSIX_Group &) const;
    int operator != (const ACE_CLASSIX_Group &) const;

    // = HELPER
    void dump(void) const;

 protected:
    // = INITIALIZATION
    ACE_CLASSIX_Group();
    // default constructor, does nothing but init.

    void init_(void);
    // Initializes the structure.

    void set_config_();
    // Sets the group  configured.
    void clear_config_();
    // Sets the group  not-configured.

    struct group_addr
    {
        int              config_; // 1, if the following fields are valid
        KnCap            group_;
    };
    group_addr        addr_;

 private:

    // disable copy/assignment constructor
    ACE_CLASSIX_Group(ACE_CLASSIX_Group const&);
    ACE_CLASSIX_Group const& operator=(ACE_CLASSIX_Group const&);
};

/* ------------------------------------------------------------------------- */

#if defined (__ACE_INLINE__)
#include "ace/CLASSIX/CLASSIX_Group.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_CLASSIX_GROUP_H */
