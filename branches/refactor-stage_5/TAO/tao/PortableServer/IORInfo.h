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

#include /**/ "ace/pre.h"

#include "tao/IORInterceptor/IORInterceptorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/// Forward declarations.
class TAO_POA;


/**
 * @class TAO_IORInfo
 *
 * @brief This class exposes an interface that allows IORInterceptors add
 * tagged components to IORs.
 */
class TAO_IORInfo
  : public virtual PortableInterceptor::IORInfo,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_IORInfo (TAO_POA *poa);

  /**
   * @name PortableInterceptor::IORInfo Methods
   *
   * Methods exposed by the PortableInterceptor::IORInfo interface.
   */
  //@{

  /// Return the policy matching the given policy type that is in
  /// effect for the object whose IOR is being created.
  virtual CORBA::Policy_ptr get_effective_policy (
      CORBA::PolicyType type
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the given tagged component to all profiles.
  virtual void add_ior_component (
      const IOP::TaggedComponent & component
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the given tagged component to all profiles matching the given
  /// ProfileId.
  virtual void add_ior_component_to_profile (
      const IOP::TaggedComponent & component,
      IOP::ProfileId profile_id
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual PortableInterceptor::AdapterManagerId manager_id (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual PortableInterceptor::AdapterState state (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual PortableInterceptor::ObjectReferenceTemplate * adapter_template (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual PortableInterceptor::ObjectReferenceFactory * current_factory (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void current_factory (
      PortableInterceptor::ObjectReferenceFactory * current_factory
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  //@}

  /// Invalidate this IORInfo instance.
  /**
   * Once the IOR interception points have been invoked, this IORInfo
   * instance is no longer valid.
   */
  void invalidate (void);

  /// Inform the this IORInfo object that the
  /// IORInterceptor::components_established() interception point has
  /// been called.
  /**
   * This method is used so to inform the IORInfo when the
   * add_ior_component() and add_ior_component_to_profile() methods
   * are invalid.  They are only valid in the
   * IORInterceptor::establish_components() interception point.
   */
  void components_established (void);

protected:

  /// Protected destructor to enforce proper memory managment through
  /// the reference counting mechanism.
  ~TAO_IORInfo (void);

  /// Check if this IORInfo instance is valid.
  /**
   * Once all IORInterceptor interception points have been called,
   * this IORInfo object is no longer valid.  Throw an exception in
   * that case.
   */
  void check_validity (ACE_ENV_SINGLE_ARG_DECL);

private:

  /// Prevent copying through the copy constructor and the assignment
  /// operator.
  ACE_UNIMPLEMENTED_FUNC (
    TAO_IORInfo (const TAO_IORInfo &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_IORInfo &))

private:

  /// Pointer to POA
  TAO_POA *poa_;

  /// True if the IORInterceptor::components_established()
  /// interception point was called.  False otherwise.
  /**
   * This flag is used to prevent the add_ior_component() and
   * add_ior_component_to_profile() methods from being incorrectly
   * called after the IORInterceptor::establish_components()
   * interception point has been called.
   */
  CORBA::Boolean components_established_;

};

#if defined (__ACE_INLINE__)
# include "IORInfo.inl"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_IOR_INFO_H */
