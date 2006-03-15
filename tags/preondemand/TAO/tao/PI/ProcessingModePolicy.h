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

#include "tao/PI/pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "tao/PI/ProcessingModePolicyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ProcessingModePolicy
 *
 * @brief Implementation class for Portable Interceptor ProcessingModePolicy.
 *
 * This policy is used to specify which kinds of requests (collocated vs.
 * remote) should (or should not) cause a Portable Interceptor to be used.
 */

class TAO_PI_Export TAO_ProcessingModePolicy
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

private:
  /// The attribute
  PortableInterceptor::ProcessingMode processing_mode_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"
#endif /* TAO_PROCESSING_MODE_POLICY_H */
