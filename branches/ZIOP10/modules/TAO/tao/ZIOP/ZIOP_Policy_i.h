// -*- C -*-

// ===================================================================
/**
 *  @file ZIOP_Policy_i.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
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
class CompressorIdListPolicy
  : public virtual ::ZIOP::CompressorIdListPolicy
  , public virtual ::CORBA::LocalObject
{
public:

  /// Constructor.
  CompressorIdListPolicy (::Compression::CompressorIdList* val);

  /// Copy constructor.
  CompressorIdListPolicy (const CompressorIdListPolicy &rhs);

  /// Returns a copy of <this>.
  virtual CompressorIdListPolicy *clone (void) const;

  virtual ::Compression::CompressorIdList * compressor_ids ();

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

private:

  /// The attribute
  ::Compression::CompressorIdList* value_;
};

/**
 * @class CompressionEnablingPolicy
 *
 * @brief  Implementation of the ZIOP::CompressionEnablingPolicy
 */
class CompressionEnablingPolicy
  : public virtual ::ZIOP::CompressionEnablingPolicy
  , public virtual ::CORBA::LocalObject
{
public:

  /// Constructor.
  CompressionEnablingPolicy (const ::CORBA::Boolean val);

  /// Copy constructor.
  CompressionEnablingPolicy (const CompressionEnablingPolicy &rhs);

  /// Returns a copy of <this>.
  virtual CompressionEnablingPolicy *clone (void) const;

  /// = The ZIOP::BidirectionalPolicy methods
  virtual ::CORBA::Boolean compression_enabled (void);

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

private:

  /// The attribute
  ::CORBA::Boolean value_;
};

/**
 * @class CompressionLowValuePolicy
 *
 * @brief  Implementation of the ZIOP::CompressionLowValuePolicy
 */
class CompressionLowValuePolicy
  : public virtual ::ZIOP::CompressionLowValuePolicy
  , public virtual ::CORBA::LocalObject
{
public:

  /// Constructor.
  CompressionLowValuePolicy (const ::CORBA::ULong val);

  /// Copy constructor.
  CompressionLowValuePolicy (const CompressionLowValuePolicy &rhs);

  /// Returns a copy of <this>.
  virtual CompressionLowValuePolicy *clone (void) const;

  /// = The ZIOP::BidirectionalPolicy methods
  virtual ::CORBA::ULong low_value (void);

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

private:

  /// The attribute
  ::CORBA::ULong value_;
};

/**
 * @class CompressionLowValuePolicy
 *
 * @brief  Implementation of the ZIOP::CompressionLowValuePolicy
 */
class CompressionMinRatioPolicy
  : public virtual ::ZIOP::CompressionMinRatioPolicy
  , public virtual ::CORBA::LocalObject
{
public:

  /// Constructor.
  CompressionMinRatioPolicy (const ::CORBA::ULong val);

  /// Copy constructor.
  CompressionMinRatioPolicy (const CompressionMinRatioPolicy &rhs);

  /// Returns a copy of <this>.
  virtual CompressionMinRatioPolicy *clone (void) const;

  /// = The ZIOP::BidirectionalPolicy methods
  virtual ::CORBA::ULong ratio (void);

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

private:

  /// The attribute
  ::CORBA::ULong value_;
};
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_BIDIR_POLICY_I_H */


