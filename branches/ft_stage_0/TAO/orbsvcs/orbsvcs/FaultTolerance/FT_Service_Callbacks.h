//=============================================================================
/**
 * @file FT_Service_Callbacks.h
 *
 * $Id$
 *
 * A concrete FT service callback implementation
 *
 * @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_FT_CALLBACKS_H
#define TAO_FT_CALLBACKS_H
#include "ace/pre.h"

#include "fault_tol_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Service_Callbacks.h"
#include "orbsvcs/FT_CORBA_ORBC.h"

class TAO_Profile;
class TAO_MProfile;
class TAO_ORB_Core;

/**
 * @class TAO_FT_Service_Callbacks
 *
 * @brief A class to dynamically load the FT callback implementations in
 * to the ORB.
 *
 * An implementation of the service callbacks that allows the
 * loading of the FT ORB level library in to the ORB
 */
class TAO_FT_Export TAO_FT_Service_Callbacks : public TAO_Service_Callbacks
{
public:
  /// Constructor
  TAO_FT_Service_Callbacks (TAO_ORB_Core *orb_core);

  /// Dtor
  virtual ~TAO_FT_Service_Callbacks (void);

  /// Check whether <obj> is nil or not. FT spec suggests some
  /// extensions for a CORBA::is_nil () operation.
  virtual CORBA::Boolean object_is_nil (CORBA::Object_ptr obj);

  // Need to do is_equivalent and hash also here

  /// Check whether we need to raise an exception or go for a
  /// reinvocaton.
  virtual int raise_comm_failure (
      TAO_GIOP_Invocation *invoke,
      TAO_Profile *profile
      ACE_ENV_ARG_DECL);

  /// Check whether we need to raise an exception or go for a
  /// reinvocaton.
  virtual int raise_transient_failure (
      TAO_GIOP_Invocation *invoke,
      TAO_Profile *profile
      ACE_ENV_ARG_DECL);

private:

  /// Check whether the right flags are available so that we can issue a
  /// restart.
  CORBA::Boolean restart_policy_check (IOP::ServiceContextList &service_list,
                                       const TAO_Profile *profile);

private:
  /// The ORB core in which we have been activated
  TAO_ORB_Core *orb_core_;

  /// Mutex to protect access to the profile that gets passed along
  ACE_Lock* profile_lock_;

  /**
   * A flag that indicates that the primary has already failed. So any
   * more calls to select_profile () should not reset the profile
   * pointer to the primary.
   */
  CORBA::Boolean primary_failed_;

  /**
   * A flag to indicate that a secondary has been selected for
   * invocation. We do this only once in the cycle. When we get there
   * again we dont select a primary again and again.
   */
  CORBA::Boolean secondary_set_;

  /// FT group component in the IOGR
  FT::TagFTGroupTaggedComponent group_component_;

  /// A flag to indicate whether we have extracted the
  /// <group_component> before.
  CORBA::Boolean group_component_flag_;
};

#if defined (__ACE_INLINE__)
# include "FT_Service_Callbacks.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_FT_CALLBACKS_H*/
