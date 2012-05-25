// -*- C -*-

// ===================================================================
/**
 *  @file ZIOP_Policy_i.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen - jwillemsen@remedy.nl
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
class CompressorIdLevelListPolicy
  : public virtual ::ZIOP::CompressorIdLevelListPolicy
  , public virtual ::CORBA::LocalObject
{
public:
  CompressorIdLevelListPolicy (void);

  /// Constructor.
  CompressorIdLevelListPolicy (const ::Compression::CompressorIdLevelList& val);

  /// Copy constructor.
  CompressorIdLevelListPolicy (const CompressorIdLevelListPolicy &rhs);

  /// Returns a copy of this CompressorIdLevelListPolicy.
  virtual CompressorIdLevelListPolicy *clone (void) const;

  virtual ::Compression::CompressorIdLevelList * compressor_ids ();

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  TAO_Policy_Scope _tao_scope (void) const;

  /// This method writes a CDR representation of the current object.
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /// This method reads the object state from a CDR representation.
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

private:
  /// The attribute
  ::Compression::CompressorIdLevelList value_;
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
  CompressionEnablingPolicy (void);

  /// Constructor.
  CompressionEnablingPolicy (const ::CORBA::Boolean val);

  /// Copy constructor.
  CompressionEnablingPolicy (const CompressionEnablingPolicy &rhs);

  /// Returns a copy of this CompressionEnablingPolicy.
  virtual CompressionEnablingPolicy *clone (void) const;

  /// = The ZIOP::BidirectionalPolicy methods
  virtual ::CORBA::Boolean compression_enabled (void);

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  TAO_Policy_Scope _tao_scope (void) const;

  /// This method writes a CDR representation of the current object.
  CORBA::Boolean _tao_encode (TAO_OutputCDR &out_cdr);

  /// This method reads the object state from a CDR representation.
  CORBA::Boolean _tao_decode (TAO_InputCDR &in_cdr);

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

  /// Returns a copy of this CompressionLowValuePolicy.
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
 * @class CompressionMinRatioPolicy
 *
 * @brief  Implementation of the ZIOP::CompressionMinRatioPolicy
 */
class CompressionMinRatioPolicy
  : public virtual ::ZIOP::CompressionMinRatioPolicy
  , public virtual ::CORBA::LocalObject
{
public:

  /// Constructor.
  CompressionMinRatioPolicy (const ::Compression::CompressionRatio val);

  /// Copy constructor.
  CompressionMinRatioPolicy (const CompressionMinRatioPolicy &rhs);

  /// Returns a copy of this CompressionMinRatioPolicy.
  virtual CompressionMinRatioPolicy *clone (void) const;

  virtual ::Compression::CompressionRatio ratio (void);

  virtual CORBA::PolicyType policy_type (void);

  virtual CORBA::Policy_ptr copy (void);

  virtual void destroy (void);

  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

private:

  /// The attribute
  ::Compression::CompressionRatio value_;
};
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_BIDIR_POLICY_I_H */


