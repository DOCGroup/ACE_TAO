// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     MProfile.h
 *
 *  $Id$
 *
 *   Keep track of profile lists
 *
 *
 *  @author  Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_MPROFILE_H
#define TAO_MPROFILE_H
#include "ace/pre.h"
#include "ace/Synch.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Profile.h"

typedef CORBA::ULong TAO_PHandle;

/**
 * @class TAO_MProfile
 *
 * @brief This class implements the basic interface for supporting
 * multiple profiles.
 *
 * Multiple profiles can be treated either as a circular queue or
 * a linear array of profiles.
 * It is assumed that locking will only be required when a profile
 * list is associated with a TAO_Stub.  Thus when the
 * TAO_Stub accepts ownership of an MProfile it also assumes
 * responsibility for controling access (i.e. locking).
 */
class TAO_Export TAO_MProfile
{
public:
  // = Initialization and termination methods.
  TAO_MProfile (CORBA::ULong sz = 0);

  /**
   * **NOTE:  IF mprofiles->last_ > 0, THEN this->size_ will be set to
   *          mprofiles->last_.  Otherwise this->size_ - mprofiles->size_.
   *          Furthermore, current_ is set back to 0!  i.e. rewound.
   * The reference count on any profiles in mprofiles is increment
   * when their references (i.e. pointers) are copied.
   */
  TAO_MProfile (const TAO_MProfile &mprofiles);

  /// Assigment operator.
  TAO_MProfile& operator= (const TAO_MProfile& mprofiles);

  /// Destructor: decrements reference count on all references
  /// profiles!
  ~TAO_MProfile (void);

  /// Inits MProfile to hold sz TAO_Profiles.
  /// NOT THREAD SAFE
  int set (CORBA::ULong sz);

  /**
   * Inits this to the values of mprofile.  NOTE: We use
   * mprofile->last_ instead of mprofile->size_ to set this->size_.
   * This is so we can use set () to trim a profile list!!
   * NOT THREAD SAFE
   */
  int set (const TAO_MProfile &mprofile);

  /// increase the number of profiles this object can hold.
  /// NOT THREAD SAFE
  int grow (CORBA::ULong sz);

  /// Treat as a circular list.
  TAO_Profile *get_cnext (void);

  /// Get next profile in list, return 0 at end of list.
  TAO_Profile *get_next (void);

  /// Assume a circular list of profiles.
  TAO_Profile *get_cprev (void);

  /// Get previous profile, stop at beginning of list and return 0.
  TAO_Profile *get_prev (void);

  /// Return a pointer to the current profile, will not increment
  /// reference pointer.
  TAO_Profile *get_current_profile (void);

  /// Return a pointer to the profile referenced by handle void.
  TAO_Profile *get_profile (TAO_PHandle handle);

  // rem_profile (TAO_PHandle handle); let's wait.

  /// Returns the index for the current profile.
  TAO_PHandle get_current_handle (void);

  /// Returns the index for the current profile.
  TAO_PHandle get_current_handle (void) const;

  /// Returns the number of profiles stored in the list (last_+1).
  CORBA::ULong profile_count (void) const;

  /// return the maximum number of profiles that can be stored in this
  /// container, (size_+1)
  CORBA::ULong size (void) const;

  /// Return the profile at position <slot>.  If <slot> is out of range
  /// it returns 0.
  const TAO_Profile* get_profile (CORBA::ULong slot) const;

  /// Sets the current slot back to 0.
  void rewind (void);

  /// Return the index of this entry or -1 if it can not be added.
  /// reference count on profile in incremented!
  int add_profile (TAO_Profile *pfile);

  /// Return the index of this entry or -1 if it can not be added.
  /// this object assumes ownership of this profile!!
  int give_profile (TAO_Profile *pfile);

  /// append the profiles in pfiles to this object.  The count
  /// will be incremented on the individual profile objects.
  int add_profiles (TAO_MProfile *pfiles);

  /// remove from this MProfile any profiles which also appear in pfiles.
  int remove_profile (const TAO_Profile *pfile);

  /// remove from this MProfile any profiles which also appear in pfiles.
  int remove_profiles (const TAO_MProfile *pfiles);

  /// Set a pointer to the MProfile whose 'current' TAO_Profile was
  /// forwarded This object is the set of forwarding profiles.
  void forward_from (TAO_MProfile *mprofiles);

  /// Returns a pointer to the profile which was forwarded.
  TAO_MProfile *forward_from (void);

  /**
   * Returns true of there is at least one profile in first which
   * is_equivalent with at least one profile in second.
   * NON-THREAD SAFE, relies on some other entity to guarentee
   * the profiles will not change during the call.
   */
  CORBA::Boolean is_equivalent (const TAO_MProfile *rhs);

  /**
   * use all registered profiles.  The hash() method is called on each
   * profile and the results are averaged together.
   * NON-THREAD SAFE.
   */
  CORBA::ULong hash (CORBA::ULong max
                     ACE_ENV_ARG_DECL);

protected:

  /// This method handle the dynamic allocation of the data member
  /// <policy_list_>.
 void create_policy_list (ACE_ENV_SINGLE_ARG_DECL);

public:

  /// Sets the policies list associated with the profiles
  /// owned by the TAO_MProfile.
  void policy_list (CORBA::PolicyList *policy_list);

  /// Gets the policies list associated with the profiles
  /// owned by the TAO_MProfile.
  CORBA::PolicyList *policy_list (ACE_ENV_SINGLE_ARG_DECL);

protected:

  /// Initialize the policy list, demarsharling the policy.
  void init_policy_list (ACE_ENV_SINGLE_ARG_DECL);

protected:
  /// Stores the policy list for the profile of this MProfile.
  friend class TAO_Profile;
  CORBA::PolicyList *policy_list_;

  CORBA::Boolean is_policy_list_initialized_;

  /// Mutex used to make sure that only one policy list
  /// is created.
  TAO_SYNCH_RECURSIVE_MUTEX mutex_;

protected:
  /// return the complete list of profiles, this object retains
  /// ownership!
  TAO_Profile **pfiles (void) const;


private:
  /// Helper method to implement the destructor
  void cleanup (void);

private:

  /**
   * Used for chaning references when the current profile is
   * forwarded.  Note, this will only be valid for an MProfile which
   * contains a list of forward_profiles for some initial or base
   * profile.  This is a backward reference to the profile list which
   * received the relocate message.  The actual profile what was
   * forwarded will be forward_from_->get_current_profile ()
   */
  TAO_MProfile  *forward_from_;

  /// Actual list of profiles.
  TAO_Profile **pfiles_;

  /// Points to the next profile to be used.  0 ... size_
  TAO_PHandle current_;

  /// Max size of array
  TAO_PHandle size_;

  /// Index plus 1 of last valid entry!  May be < size_.
  TAO_PHandle last_;
};

#if defined (__ACE_INLINE__)
# include "tao/MProfile.i"
#endif /*__ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_MPROFILE_H */
