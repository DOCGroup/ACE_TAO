// $Id$


#include "Profile.h"
#include "Object_KeyC.h"

#include "Messaging_PolicyValueC.h"
#include "Stub.h"
#include "debug.h"
#include "tao/target_specification.h"

#if !defined (__ACE_INLINE__)
#include "Profile.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Profile, "$Id$")


// ****************************************************************

TAO_Profile::~TAO_Profile (void)
{
}

void
TAO_Profile::add_tagged_component (const IOP::TaggedComponent &component
                                   ACE_ENV_ARG_DECL)
{
  // Sanity checks.
  this->verify_orb_configuration (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->verify_profile_version (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ----------------------------------------------------------------

  // Add the given tagged component to this profile.
  this->tagged_components ().set_component (component);
}

void
TAO_Profile::policies (CORBA::PolicyList *policy_list
                       ACE_ENV_ARG_DECL)
{
#if (TAO_HAS_CORBA_MESSAGING == 1)

  if (policy_list == 0)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO_Profile::policies: Null Policy List!\n")));
      return;
    }

  Messaging::PolicyValue pv;
  Messaging::PolicyValueSeq policy_value_seq;

  CORBA::ULong length;
  CORBA::Octet *buf = 0;

  policy_value_seq.length (policy_list->length ());

  // This loop iterates through CORBA::PolicyList to convert
  // each CORBA::Policy into a CORBA::PolicyValue
  for (size_t i = 0; i < policy_list->length (); ++i)
    {
      TAO_OutputCDR out_CDR;
      policy_value_seq[i].ptype =
        (*policy_list)[i]->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      out_CDR << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
      (*policy_list)[i]->_tao_encode (out_CDR);

      length = out_CDR.total_length ();
      policy_value_seq[i].pvalue.length (length);

      buf = policy_value_seq[i].pvalue.get_buffer ();

      // Copy the CDR buffer data into the sequence<octect> buffer.

      for (const ACE_Message_Block *iterator = out_CDR.begin ();
           iterator != 0;
           iterator = iterator->cont ())
        {
          ACE_OS::memcpy (buf, iterator->rd_ptr (), iterator->length ());
          buf += iterator->length ();
        }
    }

  TAO_OutputCDR out_cdr;
  // Now we have to embedd the Messaging::PolicyValueSeq into
  // a TaggedComponent.

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = Messaging::TAG_POLICIES;

  out_cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
  out_cdr << policy_value_seq;

  length = out_cdr.total_length ();

  tagged_component.component_data.length (length);
  buf = tagged_component.component_data.get_buffer ();

  for (const ACE_Message_Block *iterator = out_cdr.begin ();
       iterator != 0;
       iterator = iterator->cont ())
    {
      CORBA::ULong i_length = iterator->length ();
      ACE_OS::memcpy (buf, iterator->rd_ptr (), i_length);

      buf += i_length;
    }

  // Eventually we add the TaggedComponent to the TAO_TaggedComponents
  // member variable.
  tagged_components_.set_component (tagged_component);
  this->are_policies_parsed_ = 1;

#else /* TAO_HAS_CORBA_MESSAGING == 1 */

  ACE_UNUSED_ARG (policy_list);
  ACE_ENV_ARG_NOT_USED;  // FUZZ: ignore check_for_ace_check

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
}


CORBA::PolicyList&
TAO_Profile::policies (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
#if (TAO_HAS_CORBA_MESSAGING == 1)

  CORBA::PolicyList *policies = this->stub_->base_profiles ().policy_list_;

  if (!this->are_policies_parsed_)
    // None has already parsed the policies.
    {
      IOP::TaggedComponent tagged_component;
      tagged_component.tag = Messaging::TAG_POLICIES;

      // This gets a component with the proper "tag" field
      // if it exists.
      if (this->tagged_components_.get_component (tagged_component))
        {
          const CORBA::Octet *buf =
            tagged_component.component_data.get_buffer ();

          TAO_InputCDR in_cdr (ACE_reinterpret_cast (const char*, buf),
                               tagged_component.component_data.length ());

          // Extract the Byte Order
          CORBA::Boolean byte_order;
          if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
              return *(stub_->base_profiles ().policy_list_);
          in_cdr.reset_byte_order (ACE_static_cast(int, byte_order));

          // Now we take out the Messaging::PolicyValueSeq out from the
          // CDR.
          Messaging::PolicyValueSeq policy_value_seq;
          in_cdr >> policy_value_seq;

          // Here we extract the Messaging::PolicyValue out of the sequence
          // and we convert those into the proper CORBA::Policy

          CORBA::Policy_var policy;
          CORBA::ULong length = policy_value_seq.length ();

          // Set the policy list length.
          policies->length (length);

          for (CORBA::ULong i = 0; i < length; ++i)
            {
              ACE_TRY_NEW_ENV
                {
                  // @@ Angelo: please check my comments on this stuff
                  //            in the Policy_Factory.h file.
                  // @@ I updated this code to use the standard
                  //    ORB::create_policy () which now queries the
                  //    policy factory registry.
                  //       -Ossama

                  // We don't need to pass any policy construction
                  // value to the RT policies.
                  CORBA::Any dummy_any;
                  policy =
                    this->orb_core_->orb ()->create_policy (
                      policy_value_seq[i].ptype,
                      dummy_any
                       ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  if (!CORBA::is_nil (policy.in ()))
                    {
                      buf = policy_value_seq[i].pvalue.get_buffer ();

                      TAO_InputCDR in_cdr (
                        ACE_reinterpret_cast (const char*, buf),
                        policy_value_seq[i].pvalue.length ());

                      in_cdr >> ACE_InputCDR::to_boolean (byte_order);
                      in_cdr.reset_byte_order (ACE_static_cast(int,
                                                               byte_order));

                      policy->_tao_decode (in_cdr);
                      (*policies)[i] = policy._retn ();
                    }
                  else
                    {
                      // This case should occure when in the IOR are
                      // embedded policies that TAO doesn't support,
                      // so as specified by the RT-CORBA
                      // spec. ptc/99-05-03 we just ignore these
                      // un-understood policies.

                      if (TAO_debug_level >= 5)
                        ACE_DEBUG ((LM_DEBUG,
                                ACE_TEXT ("The IOR contains Unsupported Policies.\n")));
                    }
                }
              ACE_CATCHANY
                {
                  // This case should occure when in the IOR are
                  // embedded policies that TAO doesn't support, so as
                  // specified by the RT-CORBA spec. ptc/99-05-03 we
                  // just ignore these un-understood policies.

                  if (TAO_debug_level >= 5)
                    ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION,
                                        ACE_TEXT ("IOR contains ")
                                        ACE_TEXT ("unsupported policies."));
                }
              ACE_ENDTRY;
            }
        }
    }

#endif /* (TAO_HAS_CORBA_MESSAGING == 1) */

  return *(stub_->base_profiles ().policy_list_);
}

void
TAO_Profile::the_stub (TAO_Stub *stub)
{
  this->stub_ = stub;
}

TAO_Stub*
TAO_Profile::the_stub (void)
{
  return stub_;
}

void
TAO_Profile::verify_orb_configuration (ACE_ENV_SINGLE_ARG_DECL)
{
  // If the ORB isn't configured to support tagged components, then
  // throw an exception.
  if (this->orb_core_->orb_params ()->std_profile_components () == 0
      || !this->orb_core_->orb ()->_use_omg_ior_format ())
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Cannot add ")
                    ACE_TEXT ("IOP::TaggedComponent to profile.\n")
                    ACE_TEXT ("(%P|%t) Standard profile components ")
                    ACE_TEXT ("have been disabled or URL style IORs\n")
                    ACE_TEXT ("(%P|%t) are in use.  Try ")
                    ACE_TEXT ("\"-ORBStdProfileComponents 1\" and/or\n")
                    ACE_TEXT ("(%P|%t) \"-ORBObjRefStyle IOR\".\n")));

      // According to the Portable Interceptor specification, we're
      // supposed to throw a CORBA::BAD_PARAM exception if it isn't
      // possible to add components to the profile.
      // @todo: We need the proper minor code as soon as the spec is
      //        updated.
      ACE_THROW (CORBA::BAD_PARAM (
                   CORBA_SystemException::_tao_minor_code (
                      TAO_DEFAULT_MINOR_CODE,
                      EINVAL),
                   CORBA::COMPLETED_NO));
    }
}

void
TAO_Profile::verify_profile_version (ACE_ENV_SINGLE_ARG_DECL)
{
  // GIOP 1.0 does not support tagged components.  Throw an exception
  // if the profile is a GIOP 1.0 profile.

  if (this->version_.major == 1 && this->version_.minor == 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Cannot add ")
                    ACE_TEXT ("IOP::TaggedComponent to GIOP 1.0")
                    ACE_TEXT ("IOR profile.\n")
                    ACE_TEXT ("(%P|%t) Try using a GIOP 1.1 or ")
                    ACE_TEXT ("greater endpoint.\n")));

      // According to the Portable Interceptor specification, we're
      // supposed to throw a CORBA::BAD_PARAM exception if it isn't
      // possible to add components to the profile.
      // @todo: We need the proper minor code as soon as the spec is
      //        updated.
      ACE_THROW (CORBA::BAD_PARAM (
                   CORBA_SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                   CORBA::COMPLETED_NO));
    }
}

int
TAO_Profile::supports_multicast (void) const
{
  // Most profiles do not support multicast endpoints.
  return 0;
}


void
TAO_Profile::addressing_mode (CORBA::Short addr
                              ACE_ENV_ARG_DECL)
{
  // ** See race condition note about addressing mode in Profile.h **
  switch (addr)
    {
    case TAO_Target_Specification::Key_Addr:
    case TAO_Target_Specification::Profile_Addr:
    case TAO_Target_Specification::Reference_Addr:
      this->addressing_mode_ = addr;
      break;

    default:
      ACE_THROW (CORBA::BAD_PARAM (
             CORBA_SystemException::_tao_minor_code (
               TAO_DEFAULT_MINOR_CODE,
               EINVAL),
             CORBA::COMPLETED_NO));
    }
}


// ****************************************************************

TAO_Unknown_Profile::TAO_Unknown_Profile (CORBA::ULong tag,
                                          TAO_ORB_Core *orb_core)
  : TAO_Profile (tag,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR)),
    tagged_profile_ ()
{
}

TAO_Endpoint*
TAO_Unknown_Profile::endpoint (void)
{
  return 0;
}

size_t
TAO_Unknown_Profile::endpoint_count (void)
{
  return 0;
}

void
TAO_Unknown_Profile::parse_string (const char *
                                   ACE_ENV_ARG_DECL_NOT_USED)
{
  // @@ THROW something????
}

char
TAO_Unknown_Profile::object_key_delimiter (void) const
{
  return 0;
}

char *
TAO_Unknown_Profile::to_string (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // @@ THROW something?
  return 0;
}

int
TAO_Unknown_Profile::decode (TAO_InputCDR& cdr)
{
  if ((cdr >> this->body_) == 0)
    return -1;
  return 0;
}

int
TAO_Unknown_Profile::encode (TAO_OutputCDR &stream) const
{
  stream.write_ulong (this->tag ());
  return (stream << this->body_);
}

int
TAO_Unknown_Profile::encode_endpoints (void)
{
  return 0;
}

const TAO_ObjectKey &
TAO_Unknown_Profile::object_key (void) const
{
  // @@ TODO this is wrong, but the function is deprecated anyway....
  static TAO_ObjectKey empty_key;
  return empty_key;
}

TAO_ObjectKey *
TAO_Unknown_Profile::_key (void) const
{
  return 0;
}

CORBA::Boolean
TAO_Unknown_Profile::is_equivalent (const TAO_Profile* other_profile)
{
  if (other_profile->tag () != this->tag ())
    return 0;

  const TAO_Unknown_Profile *op =
    ACE_dynamic_cast (const TAO_Unknown_Profile*, other_profile);

  return (CORBA::Boolean) (this->body_ == op->body_);
}

CORBA::ULong
TAO_Unknown_Profile::hash (CORBA::ULong max
                           ACE_ENV_ARG_DECL_NOT_USED)
{
  return (ACE::hash_pjw (ACE_reinterpret_cast (const char*,
                                               this->body_.get_buffer ()),
                         this->body_.length ()) % max);
}

IOP::TaggedProfile&
TAO_Unknown_Profile::create_tagged_profile (void)
{
  this->tagged_profile_.tag = this->tag ();

  // I dont know about the rest, so we return our copy
  return this->tagged_profile_;

}
