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
  const IOP::ServiceContext &ctx)
{
  // Ensure this context is actually for us to decode.
  if (ctx.context_id != IOP::INVOCATION_POLICIES)
    {
      return 0;
    }

  // Create an input CDR from the context buffer
  TAO_InputCDR cdr (reinterpret_cast<const char*> (ctx.context_data.get_buffer ()),
                    ctx.context_data.length () );
  CORBA::Boolean byte_order;
  if (!(cdr >> TAO_InputCDR::to_boolean (byte_order)))
    {
      return 0;
    }
  cdr.reset_byte_order (static_cast<int> (byte_order));

  // Extract the sequence of policy values that was sent.
  Messaging::PolicyValueSeq policy_value_seq;
  if (!(cdr >> policy_value_seq))
    {
      return 0;
    }

  // Extract each policy from the slots we recieved.
  for (size_t i = 0u; i < policy_value_seq.length (); ++i)
    {
      TAO_InputCDR policy_cdr (
        reinterpret_cast<const char*> (policy_value_seq[i].pvalue.get_buffer ()),
        policy_value_seq[i].pvalue.length ());

      if (policy_cdr >> TAO_InputCDR::to_boolean (byte_order))
        {
          policy_cdr.reset_byte_order (static_cast<int> (byte_order));
          switch (policy_value_seq[i].ptype)
            {
            case ::ZIOP::COMPRESSION_ENABLING_POLICY_ID:
              {
                TAO::CompressionEnablingPolicy enabled;
                if (enabled._tao_decode (policy_cdr))
                  {
                    // TODO deal with this information sent from the client
                    // ACE_DEBUG ((LM_DEBUG, "*** ENABLING_POLICY decoded ***\n"));
                  }
                break;
              }

            case ::ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID:
              {
                TAO::CompressorIdLevelListPolicy id_list;
                if (id_list._tao_decode (policy_cdr))
                  {
                    // TODO deal with this information sent from the client
                    // ACE_DEBUG ((LM_DEBUG, "*** ID_LEVEL_LIST decoded ***\n"));
                  }
                break;
              }

            default:
              break;
            }
        }
    }

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
      // Obtain the two policies we are interested in sending to the server.
      CORBA::Policy_var idpolicy =
        stub->get_cached_policy (TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY);
      CORBA::Policy_var enabledpolicy =
        stub->get_cached_policy (TAO_CACHED_COMPRESSION_ENABLING_POLICY);

      // upcast to the actual policy types.
      ::ZIOP::CompressorIdLevelListPolicy_var idpolicyp =
         ::ZIOP::CompressorIdLevelListPolicy::_narrow (idpolicy.in ());
      ::ZIOP::CompressionEnablingPolicy_var enabledpolicyp =
         ::ZIOP::CompressionEnablingPolicy::_narrow (enabledpolicy.in ());

      // Put these into a sequence of policy values.
      Messaging::PolicyValueSeq policy_value_seq;
      policy_value_seq.length (0);
      CORBA::ULong i = 0;
      size_t length = 0;

      // If we do have an COMPRESSION_ID_LEVEL_LIST_POLICY, place it in a slot
      if (!CORBA::is_nil (idpolicyp.in ()))
        {
          policy_value_seq.length (i + 1);
          policy_value_seq[i].ptype = idpolicyp->policy_type ();

          TAO_OutputCDR out_CDR;
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

      // If we do have an COMPRESSION_ENABLING_POLICY, place it in a slot
      if (!CORBA::is_nil (enabledpolicyp.in ()))
        {
          policy_value_seq.length (i + 1);
          policy_value_seq[i].ptype = enabledpolicyp->policy_type ();

          TAO_OutputCDR out_CDR;
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

      // If we actually have any policies to send, encode them into the context.
      if (0 < policy_value_seq.length ())
        {
          TAO_OutputCDR out_cdr;
          if (!(out_cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)))
            {
              return 0;
            }

          if (!(out_cdr << policy_value_seq))
            {
            return 0;
            }

          opdetails.request_service_context ().set_context (IOP::INVOCATION_POLICIES, out_cdr);
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
#endif
