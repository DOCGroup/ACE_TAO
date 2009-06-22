// -*- C++ -*-

//=============================================================================
/**
 *  @file   EndpointPolicy_i.h
 *
 *  $Id$
 *
 *  @author  Yan Dai <dai_y@ociweb.com>
 */
//=============================================================================


#ifndef TAO_ENDPOINTPOLICY_I_H
#define TAO_ENDPOINTPOLICY_I_H

#include /**/ "ace/pre.h"

#include "tao/TAOC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/EndpointPolicy/EndpointPolicyC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


/**
 * @class TAO_EndpointPolicy_i
 *
 * @brief Implementation class for TAO-specific Endpoint Policy.
 *
 * This policy is used to filter the endpoints in profiles during
 * the creation of object references.
 */
class TAO_EndpointPolicy_Export TAO_EndpointPolicy_i
  : public virtual EndpointPolicy::Policy,
    public virtual ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_EndpointPolicy_i (const EndpointPolicy::EndpointList &value);

  /// Copy constructor.
  TAO_EndpointPolicy_i (const TAO_EndpointPolicy_i &rhs);

  virtual ~TAO_EndpointPolicy_i ();

  /// Returns a copy of <this>.
  virtual TAO_EndpointPolicy_i *clone (void) const;

  // = The EndpointPolicy::Policy methods

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

  // Return the cached policy type for this policy.
  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  virtual EndpointPolicy::EndpointList * value (void);

private:
  /// The attribute
  EndpointPolicy::EndpointList value_;
};



TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_ENDPOINTPOLICY_I_H */
