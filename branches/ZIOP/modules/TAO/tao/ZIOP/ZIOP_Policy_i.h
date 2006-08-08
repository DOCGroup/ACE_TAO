// -*- C++ -*-

// ===================================================================
/**
 *  @file ZIOP_Policy_i.h
 *
 *  $Id: ZIOP_Policy_i.h 71473 2006-03-10 07:19:20Z jtc $
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_ZIOP_POLICY_I_H
#define TAO_ZIOP_POLICY_I_H
#include /**/ "ace/pre.h"
#include "tao/ZIOP/ziop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ZIOP/ZIOP.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
/**
 * @class CompressorIdPolicy
 *
 * @brief  Implementation of the ZIOP::CompressorIdPolicy
 */
class CompressorIdPolicy
  : public virtual ::ZIOP::CompressorIdPolicy
  , public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  CompressorIdPolicy (const ::ZIOP::CompressorId val);

  /// Copy constructor.
  CompressorIdPolicy (const CompressorIdPolicy &rhs);

  /// Returns a copy of <this>.
  virtual CompressorIdPolicy *clone (void) const;

  /// = The ZIOP::BidirectionalPolicy methods
  virtual ::ZIOP::CompressorId compressor_id (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

private:

  /// The attribute
  ::ZIOP::CompressorId value_;

};

/**
 * @class CompressionEnablingPolicy
 *
 * @brief  Implementation of the ZIOP::CompressionEnablingPolicy
 */
class CompressionEnablingPolicy
  : public virtual ::ZIOP::CompressionEnablingPolicy
  , public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  CompressionEnablingPolicy (const ::CORBA::Boolean val);

  /// Copy constructor.
  CompressionEnablingPolicy (const CompressionEnablingPolicy &rhs);

  /// Returns a copy of <this>.
  virtual CompressionEnablingPolicy *clone (void) const;

  /// = The ZIOP::BidirectionalPolicy methods
  virtual ::CORBA::Boolean compression_enabled (
        ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual CORBA::PolicyType policy_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Policy_ptr copy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

private:

  /// The attribute
  ::CORBA::Boolean value_;

};
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_BIDIR_POLICY_I_H */
