// $Id$

#include "tao/ZIOP/ZIOP_PolicyFactory.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/ZIOP/ZIOP_Policy_i.h"

#include "tao/ORB_Constants.h"
#include "tao/AnyTypeCode/Any.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_ZIOP_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value)
{
  CORBA::Policy_ptr policy = CORBA::Policy::_nil ();

  switch (type)
  {
    case ZIOP::COMPRESSION_ENABLING_POLICY_ID:
    {
      ::CORBA::Boolean val;

      // Extract the value from the any.
      if (!(value >>= CORBA::Any::to_boolean (val)))
        {
          throw CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);
        }

      ACE_NEW_THROW_EX (policy,
                        TAO::CompressionEnablingPolicy (val),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }
  case ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID :
    {
      ::Compression::CompressorIdLevelList* val = 0;

      // Extract the value from the any.
      if (!(value >>= val))
        {
          throw CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);
        }

      ACE_NEW_THROW_EX (policy,
                        TAO::CompressorIdLevelListPolicy (*val),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }
  case ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID:
    {
      ::CORBA::ULong val;

      // Extract the value from the any.
      if (!(value >>= val))
        {
          throw CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);
        }

      ACE_NEW_THROW_EX (policy,
                        TAO::CompressionLowValuePolicy (val),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }
  case ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID:
    {
      ::Compression::CompressionRatio val;

      // Extract the value from the any.
      if (!(value >>= val))
        {
          throw CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);
        }

      ACE_NEW_THROW_EX (policy,
                        TAO::CompressionMinRatioPolicy (val),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }
  }

  throw CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

CORBA::Policy_ptr
TAO_ZIOP_PolicyFactory::_create_policy (CORBA::PolicyType type)
{
  CORBA::Policy_ptr policy = CORBA::Policy_ptr ();

  switch (type)
  {
    case ZIOP::COMPRESSION_ENABLING_POLICY_ID:
    {
      ACE_NEW_THROW_EX (policy,
                        TAO::CompressionEnablingPolicy,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }
  case ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID:
    {
      ACE_NEW_THROW_EX (policy,
                        TAO::CompressorIdLevelListPolicy,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }
  }
  throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}


TAO_END_VERSIONED_NAMESPACE_DECL

