// -*- C++ -*-

//=============================================================================
/**
 * @file FT_Service_Callbacks.h
 *
 * A concrete FT service callback implementation
 *
 * @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_FT_CALLBACKS_H
#define TAO_FT_CALLBACKS_H
#include /**/ "ace/pre.h"

#include "orbsvcs/FaultTolerance/FT_ClientORB_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Service_Callbacks.h"
#include "orbsvcs/FT_CORBA_ORBC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
class TAO_FT_ClientORB_Export TAO_FT_Service_Callbacks : public TAO_Service_Callbacks
{
public:
  /// Constructor
  TAO_FT_Service_Callbacks (TAO_ORB_Core *);

  /// Dtor
  ~TAO_FT_Service_Callbacks (void) override;

  /// Select the profile from MProfile as the needs of the services
  /// may be. Return the profile in @a pfile
  CORBA::Boolean select_profile (const TAO_MProfile &mprofile,
                                         TAO_Profile *&pfile) override;

  /// Check whether <obj> is nil or not. FT spec suggests some
  /// extensions for a CORBA::is_nil () operation.
  CORBA::Boolean object_is_nil (CORBA::Object_ptr obj) override;

  /// Check for equivalency of the two profiles
  TAO_Service_Callbacks::Profile_Equivalence is_profile_equivalent (
                                                const TAO_Profile *,
                                                const TAO_Profile *) override;
  /// Calculate the hash
  CORBA::ULong hash_ft (TAO_Profile *p,
                                CORBA::ULong m) override;


  /// Verify condition for  permanent forward is given,
  /// both parameters must provide group attributes.
  CORBA::Boolean is_permanent_forward_condition
  (const CORBA::Object_ptr obj,
   const TAO_Service_Context &service_context) const override;

  /// Check whether we need to raise an exception or go for a
  /// reinvocaton.
  TAO::Invocation_Status raise_comm_failure (
      IOP::ServiceContextList &clist,
      TAO_Profile *profile) override;

  /// Check whether we need to raise an exception or go for a
  /// reinvocaton.
  TAO::Invocation_Status raise_transient_failure (
      IOP::ServiceContextList &clist,
      TAO_Profile *profile) override;

  static TimeBase::TimeT now (void);

private:
  /// Check whether the right flags are available so that we can issue a
  /// restart.
  CORBA::Boolean restart_policy_check (IOP::ServiceContextList &service_list,
                                       const TAO_Profile *profile);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /*TAO_FT_CALLBACKS_H*/
