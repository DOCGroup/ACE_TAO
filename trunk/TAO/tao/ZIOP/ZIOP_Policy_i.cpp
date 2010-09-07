// $Id$

#include "tao/ZIOP/ZIOP_Policy_i.h"
#include "tao/CDR.h"
#include "tao/Stub.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
CompressorIdLevelListPolicy::CompressorIdLevelListPolicy (
    const ::Compression::CompressorIdLevelList& val)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::ZIOP::CompressorIdLevelListPolicy ()
  , ::CORBA::LocalObject ()
  , value_ (val)
{
}

CompressorIdLevelListPolicy::CompressorIdLevelListPolicy (void)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::ZIOP::CompressorIdLevelListPolicy ()
  , ::CORBA::LocalObject ()
  , value_ (0)
{
}

CompressorIdLevelListPolicy::CompressorIdLevelListPolicy (const CompressorIdLevelListPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::ZIOP::CompressorIdLevelListPolicy ()
  , ::CORBA::LocalObject ()
  , value_ (rhs.value_)
{
}

CORBA::PolicyType
CompressorIdLevelListPolicy::policy_type (void)
{
  // Future policy implementors: notice how this minimizes the
  // footprint of the class.
  return ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID;
}


CompressorIdLevelListPolicy *
CompressorIdLevelListPolicy::clone (void) const
{
  CompressorIdLevelListPolicy *copy = 0;
  ACE_NEW_RETURN (copy,
                  CompressorIdLevelListPolicy (*this),
                  0);
  return copy;
}

CORBA::Policy_ptr
CompressorIdLevelListPolicy::copy (void)
{
  CompressorIdLevelListPolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp, CompressorIdLevelListPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
CompressorIdLevelListPolicy::destroy (void)
{
}

::Compression::CompressorIdLevelList *
CompressorIdLevelListPolicy::compressor_ids (void)
{
  return &this->value_;
}

TAO_Cached_Policy_Type
CompressorIdLevelListPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY;
}

TAO_Policy_Scope
CompressorIdLevelListPolicy::_tao_scope (void) const
{
  return static_cast<TAO_Policy_Scope> (TAO_POLICY_DEFAULT_SCOPE |
                                        TAO_POLICY_CLIENT_EXPOSED);
}

CORBA::Boolean
CompressorIdLevelListPolicy::_tao_encode (TAO_OutputCDR &out_cdr)
{
  return out_cdr << this->value_;
}

CORBA::Boolean
CompressorIdLevelListPolicy::_tao_decode (TAO_InputCDR &in_cdr)
{
  return in_cdr >> this->value_;
}

CompressionEnablingPolicy::CompressionEnablingPolicy (
    const ::CORBA::Boolean val)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::ZIOP::CompressionEnablingPolicy ()
  , ::CORBA::LocalObject ()
  , value_ (val)
{
}

CompressionEnablingPolicy::CompressionEnablingPolicy (void)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::ZIOP::CompressionEnablingPolicy ()
  , ::CORBA::LocalObject ()
  , value_ (false)
{
}

CompressionEnablingPolicy::CompressionEnablingPolicy (const CompressionEnablingPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::ZIOP::CompressionEnablingPolicy ()
 , ::CORBA::LocalObject ()
  , value_ (rhs.value_)
{
}

CORBA::PolicyType
CompressionEnablingPolicy::policy_type (void)
{
  // Future policy implementors: notice how this minimizes the
  // footprint of the class.
  return ZIOP::COMPRESSION_ENABLING_POLICY_ID;
}


CompressionEnablingPolicy *
CompressionEnablingPolicy::clone (void) const
{
  CompressionEnablingPolicy *copy = 0;
  ACE_NEW_RETURN (copy,
                  CompressionEnablingPolicy (*this),
                  0);
  return copy;
}

CORBA::Policy_ptr
CompressionEnablingPolicy::copy (void)
{
  // Future policy implementors: notice how the following code is
 // exception safe!

  CompressionEnablingPolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp, CompressionEnablingPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
CompressionEnablingPolicy::destroy (void)
{
}

::CORBA::Boolean
CompressionEnablingPolicy::compression_enabled (void)
{
  return this->value_;
}


TAO_Cached_Policy_Type
CompressionEnablingPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_COMPRESSION_ENABLING_POLICY;
}

TAO_Policy_Scope
CompressionEnablingPolicy::_tao_scope (void) const
{
  return static_cast<TAO_Policy_Scope> (TAO_POLICY_DEFAULT_SCOPE |
                          TAO_POLICY_CLIENT_EXPOSED);
}

CORBA::Boolean
CompressionEnablingPolicy::_tao_encode (TAO_OutputCDR &out_cdr)
{
  return out_cdr << ACE_OutputCDR::from_boolean (this->value_);
}

CORBA::Boolean
CompressionEnablingPolicy::_tao_decode (TAO_InputCDR &in_cdr)
{
  return in_cdr >> ACE_InputCDR::to_boolean (this->value_);
}

CompressionLowValuePolicy::CompressionLowValuePolicy (
    const ::CORBA::ULong val)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::ZIOP::CompressionLowValuePolicy ()
  , ::CORBA::LocalObject ()
  , value_ (val)
{
}

CompressionLowValuePolicy::CompressionLowValuePolicy (const CompressionLowValuePolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::ZIOP::CompressionLowValuePolicy ()
 , ::CORBA::LocalObject ()
  , value_ (rhs.value_)
{
}

CORBA::PolicyType
CompressionLowValuePolicy::policy_type (void)
{
  // Future policy implementors: notice how this minimizes the
  // footprint of the class.
  return ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID;
}


CompressionLowValuePolicy *
CompressionLowValuePolicy::clone (void) const
{
  CompressionLowValuePolicy *copy = 0;
  ACE_NEW_RETURN (copy,
                  CompressionLowValuePolicy (*this),
                  0);
  return copy;
}

CORBA::Policy_ptr
CompressionLowValuePolicy::copy (void)
{
  // Future policy implementors: notice how the following code is
 // exception safe!

  CompressionLowValuePolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp, CompressionLowValuePolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
CompressionLowValuePolicy::destroy (void)
{
}

::CORBA::ULong
CompressionLowValuePolicy::low_value (void)
{
  return this->value_;
}


TAO_Cached_Policy_Type
CompressionLowValuePolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_COMPRESSION_LOW_VALUE_POLICY;
}

CompressionMinRatioPolicy::CompressionMinRatioPolicy (
    const ::Compression::CompressionRatio val)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::ZIOP::CompressionMinRatioPolicy ()
  , ::CORBA::LocalObject ()
  , value_ (val)
{
}

CompressionMinRatioPolicy::CompressionMinRatioPolicy (const CompressionMinRatioPolicy &rhs)
  : ::CORBA::Object ()
  , ::CORBA::Policy ()
  , ::ZIOP::CompressionMinRatioPolicy ()
 , ::CORBA::LocalObject ()
  , value_ (rhs.value_)
{
}

CORBA::PolicyType
CompressionMinRatioPolicy::policy_type (void)
{
  // Future policy implementors: notice how this minimizes the
  // footprint of the class.
  return ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID;
}


CompressionMinRatioPolicy *
CompressionMinRatioPolicy::clone (void) const
{
  CompressionMinRatioPolicy *copy = 0;
  ACE_NEW_RETURN (copy,
                  CompressionMinRatioPolicy (*this),
                  0);
  return copy;
}

CORBA::Policy_ptr
CompressionMinRatioPolicy::copy (void)
{
  // Future policy implementors: notice how the following code is
 // exception safe!

  CompressionMinRatioPolicy* tmp = 0;
  ACE_NEW_THROW_EX (tmp, CompressionMinRatioPolicy (*this),
                    CORBA::NO_MEMORY (TAO::VMCID,
                                      CORBA::COMPLETED_NO));

  return tmp;
}

void
CompressionMinRatioPolicy::destroy (void)
{
}

::Compression::CompressionRatio
CompressionMinRatioPolicy::ratio (void)
{
  return this->value_;
}


TAO_Cached_Policy_Type
CompressionMinRatioPolicy::_tao_cached_type (void) const
{
  return TAO_CACHED_MIN_COMPRESSION_RATIO_POLICY;
}

}
TAO_END_VERSIONED_NAMESPACE_DECL

