#include "tao/ZIOP/ZIOP_PolicyFactory.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/ZIOP/ZIOP_Policy_i.h"

#include "tao/ORB_Constants.h"
#include "tao/AnyTypeCode/Any.h"

ACE_RCSID (ZIOP_GIOP,
           ZIOP_PolicyFactory,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_ZIOP_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value)
{
  CORBA::Policy_ptr policy = CORBA::Policy::_nil ();

  if (type == ZIOP::COMPRESSION_ENABLING_POLICY_ID)
    {
      ::CORBA::Boolean val;

      // Extract the value from the any.

      if ((value >>= CORBA::Any::to_boolean (val)) == 0)
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
  else if (type == ZIOP::COMPRESSION_ID_LIST_POLICY_ID) {
      ::Compression::CompressorIdList* val;

      // Extract the value from the any.
      if ((value >>= val) == 0)
        {
          throw CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);
        }

      ACE_NEW_THROW_EX (policy,
                        TAO::CompressorIdListPolicy (val),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));

      return policy;
    }
  else if (type == ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID)
    {
      ::CORBA::ULong val;

      // Extract the value from the any.

      if ((value >>= val) == 0)
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
  else if (type == ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID)
    {
      ::CORBA::ULong val;

      // Extract the value from the any.

      if ((value >>= val) == 0)
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

  throw CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
}

TAO_END_VERSIONED_NAMESPACE_DECL

