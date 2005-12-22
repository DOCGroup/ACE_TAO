/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   ProcessingModePolicy.h
 *
 *  $Id$
 *
 *  @author Tim Bradley (bradley_t@ociweb.com)
 */
//=============================================================================

#ifndef TAO_PROCESSING_MODE_POLICY_H
#define TAO_PROCESSING_MODE_POLICY_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ProcessingModePolicyC.h"

// TBD - Should this be wrapped in a check for (TAO_HAS_MINIMUM_POA == 0)?


/**
 * @class TAO_ProcessingModePolicy
 *
 * @brief Implementation class for Portable Interceptor ProcessingModePolicy.
 *
 * This policy is used to specify which kinds of requests (collocated vs.
 * remote) should (or should not) cause a Portable Interceptor to be used.
 */

class TAO_Export TAO_ProcessingModePolicy
  : public PortableInterceptor::ProcessingModePolicy,
    public TAO_Local_RefCounted_Object
{
public:
  /// Constructor.
  TAO_ProcessingModePolicy (PortableInterceptor::ProcessingMode mode);

  virtual PortableInterceptor::ProcessingMode processing_mode
                                        (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // TBD - Should these be overridden?

  // Return the cached policy type for this policy.
  // virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Return the scope at which this policy can be applied. See orbconf.h.
  // virtual TAO_Policy_Scope _tao_scope (void) const;

private:
  /// The attribute
  PortableInterceptor::ProcessingMode processing_mode_;
};

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"
#endif /* TAO_PROCESSING_MODE_POLICY_H */
