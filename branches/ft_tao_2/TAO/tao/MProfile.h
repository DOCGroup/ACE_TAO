// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     MProfile.h
//
// = DESCRIPTION
//     Keep track of profile lists
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_MPROFILE_H
#define TAO_MPROFILE_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Profile.h"

typedef CORBA::ULong TAO_PHandle;

class TAO_Export TAO_MProfile
{
  // = TITLE
  //   This class implements the basic interface for supporting
  //   multiple profiles.
  //
  // = DESCRIPTION
  //   Multiple profiles can be treated either as a circular queue or
  //   a linear array of profiles.
  //
  //   It is assumed that locking will only be required when a profile
  //   list is associated with a TAO_Stub.  Thus when the
  //   TAO_Stub accepts ownership of an MProfile it also assumes
  //   responsibility for controling access (i.e. locking).
public:
  // = Initialization and termination methods.
  TAO_MProfile (CORBA::ULong sz = 0);

  TAO_MProfile (const TAO_MProfile &mprofiles);
  // **NOTE:  IF mprofiles->last_ > 0, THEN this->size_ will be set to
  //          mprofiles->last_.  Otherwise this->size_ - mprofiles->size_.
  //          Furthermore, current_ is set back to 0!  i.e. rewound.
  // The reference count on any profiles in mprofiles is increment
  // when their references (i.e. pointers) are copied.

  TAO_MProfile& operator= (const TAO_MProfile& mprofiles);
  // Assigment operator.

  ~TAO_MProfile (void);
  // Destructor: decrements reference count on all references
  // profiles!

  int set (CORBA::ULong sz);
  // Inits MProfile to hold sz TAO_Profiles.
  // NOT THREAD SAFE

  int set (const TAO_MProfile &mprofile);
  // Inits this to the values of mprofile.  NOTE: We use
  // mprofile->last_ instead of mprofile->size_ to set this->size_.
  // This is so we can use set () to trim a profile list!!
  // NOT THREAD SAFE

  int grow (CORBA::ULong sz);
  // increase the number of profiles this object can hold.
  // NOT THREAD SAFE

  TAO_Profile *get_cnext (void);
  // Treat as a circular list.

  TAO_Profile *get_next (void);
  // Get next profile in list, return 0 at end of list.

  TAO_Profile *get_cprev (void);
  // Assume a circular list of profiles.

  TAO_Profile *get_prev (void);
  // Get previous profile, stop at beginning of list and return 0.

  TAO_Profile *get_current_profile (void);
  // Return a pointer to the current profile, will not increment
  // reference pointer.

  TAO_Profile *get_profile (TAO_PHandle handle);
  // Return a pointer to the profile referenced by handle void.

  // rem_profile (TAO_PHandle handle); let's wait.

  TAO_PHandle get_current_handle (void);
  // Returns the index for the current profile.

  TAO_PHandle get_current_handle (void) const;
  // Returns the index for the current profile.

  CORBA::ULong profile_count (void) const;
  // Returns the number of profiles stored in the list (last_+1).

  CORBA::ULong size (void) const;
  // return the maximum number of profiles that can be stored in this
  // container, (size_+1)

  const TAO_Profile* get_profile (CORBA::ULong slot) const;
  // Return the profile at position <slot>.  If <slot> is out of range
  // it returns 0.
  
  void rewind (void);
  // Sets the current slot back to 0.

  int add_profile (TAO_Profile *pfile);
  // Return the index of this entry or -1 if it can not be added.
  // reference count on profile in incremented!

  int give_profile (TAO_Profile *pfile);
  // Return the index of this entry or -1 if it can not be added.
  // this object assumes ownership of this profile!!

  int add_profiles (TAO_MProfile *pfiles);
  // append the profiles in pfiles to this object.  The count
  // will be incremented on the individual profile objects.

  int remove_profile (const TAO_Profile *pfile);
  // remove from this MProfile any profiles which also appear in pfiles.

  int remove_profiles (const TAO_MProfile *pfiles);
  // remove from this MProfile any profiles which also appear in pfiles.

  void forward_from (TAO_MProfile *mprofiles);
  // Set a pointer to the MProfile whose 'current' TAO_Profile was
  // forwarded This object is the set of forwarding profiles.

  TAO_MProfile *forward_from (void);
  // Returns a pointer to the profile which was forwarded.

  CORBA::Boolean is_equivalent (const TAO_MProfile *rhs);
  // Returns true of there is at least one profile in first which
  // is_equivalent with at least one profile in second.
  // NON-THREAD SAFE, relies on some other entity to guarentee
  // the profiles will not change during the call.

  CORBA::ULong hash (CORBA::ULong max,
                     CORBA::Environment &ACE_TRY_ENV);
  // use all registered profiles.  The hash() method is called on each
  // profile and the results are averaged together.
  // NON-THREAD SAFE.

protected:

 void create_policy_list (void);
  // This method handle the dynamic allocation of the data member
  // <policy_list_>.

public:

  void policy_list (CORBA::PolicyList *policy_list);
  // Sets the policies list associated with the profiles
  // owned by the TAO_MProfile.
  
  CORBA::PolicyList * policy_list (void);
  // Gets the policies list associated with the profiles
  // owned by the TAO_MProfile.

protected:

  CORBA::PolicyList *policy_list_;
  // Stores the policy list for the profile of this MProfile.

protected:
  TAO_Profile **pfiles (void) const;
  // return the complete list of profiles, this object retains
  // ownership!


private:
  void cleanup (void);
  // Helper method to implement the destructor

private:

  TAO_MProfile  *forward_from_;
  // Used for chaning references when the current profile is
  // forwarded.  Note, this will only be valid for an MProfile which
  // contains a list of forward_profiles for some initial or base
  // profile.  This is a backward reference to the profile list which
  // received the relocate message.  The actual profile what was
  // forwarded will be forward_from_->get_current_profile ()

  TAO_Profile **pfiles_;
  // Actual list of profiles.

  TAO_PHandle current_;
  // Points to the next profile to be used.  0 ... size_

  TAO_PHandle size_;
  // Max size of array

  TAO_PHandle last_;
  // Index plus 1 of last valid entry!  May be < size_.
};

#if defined (__ACE_INLINE__)
# include "tao/MProfile.i"
#endif /*__ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_MPROFILE_H */
