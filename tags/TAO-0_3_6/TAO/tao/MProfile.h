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

#include "tao/corbafwd.h"

class TAO_Profile;
typedef TAO_Profile *TAO_Profile_ptr;
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
  //   list is associated with a STUB_Object.  Thus when the
  //   STUB_Object accepts ownership of an MProfile it also assumes
  //   responsibility for controling access (i.e. locking).
public:
  // = Initalization and termination methods.
  TAO_MProfile (CORBA::ULong sz);

  TAO_MProfile (TAO_MProfile *mprofiles);
  // **NOTE:  IF mprofiles->last_ > 0, THEN this->size_ will be set to 
  //          mprofiles->last_.  Otherwise this->size_ - mprofiles->size_.
  //          Furthermore, current_ is set back to 0!  i.e. rewound.
  // The reference count on any profiles in mprofiles is increment
  // when their references (i.e. pointers) are copied.

  int set (CORBA::ULong sz);
  // @@ Fred, what does this method do?

  int set (TAO_MProfile *mprofile);
  // Inits this to the values of mprofile.  NOTE: We use
  // mprofile->last_ instead of mprofile->size_ to set this->size_.
  // This is so we can use set () to trim a profile list!!

  TAO_Profile_ptr get_cnext (void);
  // Treat as a circular list.

  TAO_Profile_ptr get_next (void);
  // Get next profile in list, return 0 at end of list.

  TAO_Profile_ptr get_cprev (void);
  // Assume a circular list of profiles.

  TAO_Profile_ptr get_prev (void);
  // Get previous profile, stop at beginning of list and return 0.

  TAO_Profile_ptr get_current_profile (void);
  // Return a pointer to the current profile, will not increment
  // reference pointer.

  TAO_Profile_ptr get_profile (TAO_PHandle handle);
  // Return a pointer to the profile referenced by handle void.

  // rem_profile (TAO_PHandle handle); let's wait.

  TAO_PHandle get_current_handle (void);
  // Returns the index for the current profile.

  CORBA::ULong profile_count (void);
  // Returns the number of profiles stored in the list (last_).

  void rewind (void);
  // Sets the current index back to 0.

  int add_profile (TAO_Profile_ptr pfile);
  // Return the index of this entry or -1 if it can not be added.
  // reference count on profile in incremented!

  int give_profile (TAO_Profile_ptr pfile);
  // Return the index of this entry or -1 if it can not be added.
  // this object assumes ownership of this profile!!

  void forward_from (TAO_MProfile *mprofiles);
  // @@ Fred, can you please change the name of this method to
  // "forwarded_mprofile()"?  In general, it's better to use complete
  // names.
  // Set a pointer to the MProfile whose 'current' TAO_Profile was forwarded
  // This object is the set of forwarding profiles.

  TAO_MProfile *forward_from (void);
  // Returns a pointer to the profile which was forwarded.  

  CORBA::Boolean is_equivalent (TAO_MProfile *first,
                                TAO_MProfile *second,
                                CORBA::Environment &env);
  // Returns true of there is at least one profile in first which
  // is_equivalent with at least one profile in second.  @@ FRED: The
  // lost should be locked for this!

  CORBA::ULong hash (CORBA::ULong max,
                     CORBA::Environment &env);
  // @@ FRED: The list should be locked for this!

  ~TAO_MProfile (void);
  // Deletes this object and decrements reference count on all
  // references profiles!

protected:
  TAO_Profile_ptr *pfiles (void) const;
  // return the complete list of profiles, this object retains
  // ownership!

private:

  TAO_MProfile  *forward_from_;
  // Used for chaning references when the current profile is forwarded.
  // Note, this will only be valid for an MProfile which contains a list of
  // forward_profiles for some initial or base profile.
  // This is a backward reference to the profile list which received the 
  // relocate message.  The actual profile what was forwarded will be
  // forward_from_->get_current_profile ()

  TAO_Profile_ptr *pfiles_;
  // Actual list of profiles.

  TAO_PHandle current_;
  // Points to the next prfoile to be used.
  // 0 ... size_

  TAO_PHandle size_;
  // Max size of array

  TAO_PHandle last_;
  // Index plus 1 of last valid entry!  May be < size_
};

#if defined (__ACE_INLINE__)
# include "tao/MProfile.i"
#endif /* __ACE_INLINE__ */

// @@ Fred, please don't use #endif // ..., instead, always use #endif /* ... */
#endif // TAO_MPROFILE_H
