// -*- C++ -*-

// ===================================================================
/**
 *  @file   IORInfo.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_IOR_INFO_H
#define TAO_IOR_INFO_H

#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PortableInterceptorC.h"
#include "LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/// Forward declarations.
class TAO_Profile;
class TAO_MProfile;


/**
 * @class TAO_IORInfo
 *
 * @brief
 * This class exposes an interface that allows IORInterceptors add
 * tagged components to IORs.
 */
class TAO_Export TAO_IORInfo :
  public virtual PortableInterceptor::IORInfo,
  public virtual TAO_Local_RefCounted_Object
{
  friend class TAO_dummy_friend;

public:

  /// Constructor.
  TAO_IORInfo (TAO_ORB_Core *orb_core,
               TAO_MProfile &mp,
               CORBA::PolicyList *policy_list);

  /// Return the policy matching the given policy type that is in
  /// effect for the object whose IOR is being created.
  virtual CORBA::Policy_ptr get_effective_policy (
      CORBA::PolicyType type
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the given tagged component to all profiles.
  virtual void add_ior_component (
      const IOP::TaggedComponent & component
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the given tagged component to all profiles matching the given
  /// ProfileId.
  virtual void add_ior_component_to_profile (
      const IOP::TaggedComponent & component,
      IOP::ProfileId profile_id
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:

  /// Destructor is protected to force instantiation on the heap since
  /// it is reference counted.
  ~TAO_IORInfo (void);

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  ACE_UNIMPLEMENTED_FUNC (
    TAO_IORInfo (const TAO_IORInfo &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_IORInfo &))

private:

  /// Pointer to the ORB Core of the current ORB.
  TAO_ORB_Core *orb_core_;

  /// Reference to the profiles corresponding to the servant being
  /// created.
  TAO_MProfile &mp_;

  /// Pointer to the list of policies in effect for the servant
  /// being created.
  CORBA::PolicyList *policy_list_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_IOR_INFO_H */
