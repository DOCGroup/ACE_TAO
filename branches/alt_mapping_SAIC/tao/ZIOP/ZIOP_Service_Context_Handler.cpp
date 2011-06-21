// $Id$

#include "tao/ZIOP/ZIOP_Service_Context_Handler.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/CDR.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/operation_details.h"
#include "tao/Stub.h"
#include "tao/ZIOP/ZIOP_Policy_i.h"
#include "tao/Messaging_PolicyValueC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_ZIOP_Service_Context_Handler::process_service_context (
  TAO_Transport&,
  const IOP::ServiceContext &)
{
  return 0;
}

int
TAO_ZIOP_Service_Context_Handler::generate_service_context (
  TAO_Stub *stub,
  TAO_Transport&,
  TAO_Operation_Details &opdetails,
  TAO_Target_Specification &,
  TAO_OutputCDR &)
{
  if (stub)
    {
      CORBA::Policy_var idpolicy =
        stub->get_cached_policy (TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY);

      CORBA::Policy_var enabledpolicy =
        stub->get_cached_policy (TAO_CACHED_COMPRESSION_ENABLING_POLICY);

      ::ZIOP::CompressorIdLevelListPolicy_var idpolicyp =
         ::ZIOP::CompressorIdLevelListPolicy::_narrow (idpolicy.in ());

      ::ZIOP::CompressionEnablingPolicy_var enabledpolicyp =
         ::ZIOP::CompressionEnablingPolicy::_narrow (enabledpolicy.in ());

      Messaging::PolicyValueSeq policy_value_seq;
      policy_value_seq.length (0);
      TAO_OutputCDR out_CDR;
      CORBA::ULong i = 0;
      size_t length = 0;

      if (!CORBA::is_nil (idpolicyp.in ()))
        {
          policy_value_seq.length (i + 1);
          policy_value_seq[i].ptype = idpolicyp->policy_type ();

          if (!(out_CDR << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
            return 0;

          if (!(idpolicyp->_tao_encode (out_CDR)))
            return 0;

          length = out_CDR.total_length ();
          policy_value_seq[i].pvalue.length (static_cast <CORBA::ULong>(length));
          CORBA::Octet *buf = policy_value_seq[i].pvalue.get_buffer ();

          // Copy the CDR buffer data into the octet sequence buffer.
          for (const ACE_Message_Block *iterator = out_CDR.begin ();
             iterator != 0;
             iterator = iterator->cont ())
          {
            ACE_OS::memcpy (buf, iterator->rd_ptr (), iterator->length ());
            buf += iterator->length ();
          }
          ++i;
        }
      if (!CORBA::is_nil (enabledpolicyp.in ()))
        {
          policy_value_seq.length (i + 1);
          policy_value_seq[i].ptype = enabledpolicyp->policy_type ();

          if (!(out_CDR << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
            return 0;

          if (!(enabledpolicyp->_tao_encode (out_CDR)))
            return 0;

          length = out_CDR.total_length ();
          policy_value_seq[i].pvalue.length (static_cast <CORBA::ULong>(length));
          CORBA::Octet *buf = policy_value_seq[i].pvalue.get_buffer ();

          // Copy the CDR buffer data into the octet sequence buffer.
          for (const ACE_Message_Block *iterator = out_CDR.begin ();
             iterator != 0;
             iterator = iterator->cont ())
          {
            ACE_OS::memcpy (buf, iterator->rd_ptr (), iterator->length ());
            buf += iterator->length ();
          }
          ++i;
        }

      if (policy_value_seq.length () > 0)
        {
          TAO_OutputCDR out_cdr;
          if (!(out_cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
            return 0;

          if (!(out_cdr << policy_value_seq))
            return 0;

          opdetails.request_service_context ().set_context (IOP::INVOCATION_POLICIES, out_cdr);
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
