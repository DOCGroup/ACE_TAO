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
          ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE),
                            CORBA::Policy::_nil ());
        }

      ACE_NEW_THROW_EX (policy,
                        TAO::CompressionEnablingPolicy (val),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return policy;
    }
  else if (type == ZIOP::COMPRESSOR_ID_POLICY_ID) {
      ::Compression::CompressorId val;

      // Extract the value from the any.
      if ((value >>= val) == 0)
        {
          ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE),
                            CORBA::Policy::_nil ());
        }

      ACE_NEW_THROW_EX (policy,
                        TAO::CompressorIdPolicy (val),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return policy;
  }

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}

TAO_END_VERSIONED_NAMESPACE_DECL

