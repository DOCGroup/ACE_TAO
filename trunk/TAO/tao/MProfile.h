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
typedef TAO_Profile * TAO_Profile_ptr;
// @@ Fred, please don't create global typedefs that aren't prefixed
// by TAO_.
typedef CORBA::ULong PHandle; 

class TAO_Export TAO_MProfile 
{
  // = TITLE
  //   TAO_MProfile
  // 
  // = DESCRIPTION
  // this class implements the basic interface for supporting multiple
  // profiles.  This can be treated either as a circular queue or a
  // linear array of profiles.   
  // 
  // It is assumed that locking will only be required when a profile list
  // is associated with a STUB_Object.  Thus when the STUB_Object accepts 
  // ownership of an MProfile it also assumes responsibility for controling
  // access (i.e. locking).
  // 
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
  // Get previous profile, stop at beginning of list and return 0

  TAO_Profile_ptr get_current_profile (void);
  // return a pointer to the current profile, will not increment
  // referecne pointer.

  TAO_Profile_ptr get_profile (PHandle handle);
  // Return a pointer to the profile referenced by handle void

  // rem_profile (PHandle handle); let's wait.

  PHandle get_current_handle (void);
  // Returns the index for the current profile.

  CORBA::ULong profile_count (void);
  // Returns the number of profiles stored in the list (last_).

  void rewind (void);
  // Sets the current index back to 0.

  int add_profile (TAO_Profile_ptr pfile);
  // Return the index of this entry or -1 if it can not be added.
  // reference count on profile in incremented!

  void fwded_mprofile (TAO_MProfile *mprofiles);
  // Set a pointer to the profile which was forwarded.  NOTE, the
  // forwarding profile *MUST* be set at object creation.  And it can
  // not be altered once it's set!

  TAO_MProfile *fwded_mprofile (void);
  // Returns a pointer to the profile which was forwarded.  

  CORBA::Boolean is_equivalent (TAO_MProfile *first,
                                TAO_MProfile *second,
                                CORBA::Environment &env);
  // Returns true of there is at least one profile in first which
  // is_equivalent with at least one profile in second.  @@ FRED: The
  // lost should be locked for this!

  CORBA::ULong hash (CORBA::ULong max, CORBA::Environment &env);
  // @@ FRED: The list should be locked for this!

  ~TAO_MProfile (void);
  // Deletes this object and decrements reference count on all
  // references profiles!
protected:
  TAO_Profile_ptr *pfiles (void) const;
  // return the complete list of profiles, this object retains ownership!

private:

  TAO_MProfile  *fwded_mprofile_;
  // Used for chaning references when the current profile is forwarded.
  // Note, this will only be valid for an MProfile which contains a list of
  // fwd_profiles for some initial or base profile.
  // This is a backward reference to the profile list which received the 
  // relocate message.  The actual profile what was forwarded will be
  // fwded_mprofile_->get_current_profile ()

  TAO_Profile_ptr *pfiles_;
  // Actual list of profiles.

  PHandle current_;
  // Points to the next prfoile to be used.
  // 0 ... size_

  PHandle size_;
  // Max size of array

  PHandle last_;
  // Index plus 1 of last valid entry!  May be < size_
};

#endif // TAO_MPROFILE_H
