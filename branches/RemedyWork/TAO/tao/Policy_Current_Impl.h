// -*- C++ -*-

//=============================================================================
/**
 *  @file   Policy_Current_Impl.h
 *
 *  $Id$
 *
 * An implementation for the CORBA::PolicyManager interface.
 *
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_POLICY_CURRENT_IMPL_H
#define TAO_POLICY_CURRENT_IMPL_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#include "tao/PolicyC.h"
#include "tao/LocalObject.h"
#include "tao/Policy_Set.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Policy_Current_Impl
{
public:

  TAO_Policy_Current_Impl (void);

  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy);

  /// Obtain a single cached policy.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

  // = The CORBA::PolicyManager operations

  CORBA::PolicyList * get_policy_overrides (const CORBA::PolicyTypeSeq & ts);
  void set_policy_overrides (const CORBA::PolicyList & policies,
                             CORBA::SetOverrideType set_add);

private:
  /// The implementation.
  TAO_Policy_Set manager_impl_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Policy_Current_Impl.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_POLICY_CURRENT_IMPL_H */
