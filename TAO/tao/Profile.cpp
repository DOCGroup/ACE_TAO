// $Id$

#include "Profile.h"
#include "Object_KeyC.h"
#include "Messaging_PolicyValueC.h"
#include "Stub.h"
#include "debug.h"
#include "target_specification.h"
#include "ace/CDR_Base.h"

#if !defined (__ACE_INLINE__)
#include "Profile.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Profile,
           "$Id$")

// ****************************************************************
TAO_Profile::TAO_Profile (CORBA::ULong tag,
                          TAO_ORB_Core *orb_core,
                          const TAO::ObjectKey &obj_key,
                          const TAO_GIOP_Message_Version &version)
  : version_ (version)
    , are_policies_parsed_ (0)
    , stub_ (0)
    , policy_list_ (0)
    , addressing_mode_ (0)
    , tagged_profile_ (0)
    , object_key_ (obj_key)
    , tag_ (tag)
    , orb_core_ (orb_core)
    , forward_to_ (0)
    , refcount_ (1)
{
}

TAO_Profile::TAO_Profile (CORBA::ULong tag,
                          TAO_ORB_Core *orb_core,
                          const TAO_GIOP_Message_Version &version)
  : version_ (version)
    , are_policies_parsed_ (0)
    , stub_ (0)
    , policy_list_ (0)
    , addressing_mode_ (0)
    , tagged_profile_ (0)
    , object_key_ ()
    , tag_ (tag)
    , orb_core_ (orb_core)
    , forward_to_ (0)
    , refcount_ (1)
{
}

TAO_Profile::~TAO_Profile (void)
{
  if (this->tagged_profile_)
    delete this->tagged_profile_;
}

CORBA::ULong
TAO_Profile::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->refcount_lock_, 0);

  return this->refcount_++;
}

CORBA::ULong
TAO_Profile::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->refcount_lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // refcount is 0, so delete us!
  // delete will call our ~ destructor which in turn deletes stuff.
  delete this;
  return 0;
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
  //
  // Note that multiple tagged profiles with the same tag value may be
  // added, unless the tagged component is known to be unique by TAO.
  this->tagged_components_.set_component (component);
}

TAO::ObjectKey *
TAO_Profile::_key (void) const
{
  TAO::ObjectKey *key = 0;

  ACE_NEW_RETURN (key,
                  TAO::ObjectKey (this->object_key_),
                  0);

  return key;
}


int
TAO_Profile::encode (TAO_OutputCDR &stream) const
{
  // UNSIGNED LONG, protocol tag
  stream.write_ulong (this->tag_);

  // Create the encapsulation....
  TAO_OutputCDR encap (ACE_CDR::DEFAULT_BUFSIZE,
                       TAO_ENCAP_BYTE_ORDER,
                       this->orb_core ()->output_cdr_buffer_allocator (),
                       this->orb_core ()->output_cdr_dblock_allocator (),
                       this->orb_core ()->output_cdr_msgblock_allocator (),
                       this->orb_core ()->orb_params ()->cdr_memcpy_tradeoff (),
                       TAO_DEF_GIOP_MAJOR,
                       TAO_DEF_GIOP_MINOR);

  // Create the profile body
  this->create_profile_body (encap);

  // write the encapsulation as an octet sequence...

  stream << CORBA::ULong (encap.total_length ());
  stream.write_octet_array_mb (encap.begin ());

  return 1;
}

int
TAO_Profile::decode (TAO_InputCDR& cdr)
{
  CORBA::ULong encap_len = cdr.length ();

  // Read and verify major, minor versions, ignoring profiles
  // whose versions we don't understand.
  if (!(cdr.read_octet (this->version_.major)
        && this->version_.major == TAO_DEF_GIOP_MAJOR
        && cdr.read_octet (this->version_.minor)
        && this->version_.minor <= TAO_DEF_GIOP_MINOR))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Profile::decode - v%d.%d\n"),
                    this->version_.major,
                    this->version_.minor));
      return -1;
    }

  // Transport specific details
  if (this->decode_profile (cdr) < 0)
    return -1;

  // ... and object key.
  if ((cdr >> this->object_key_) == 0)
    return -1;

  // Tagged Components *only* exist after version 1.0!
  // For GIOP 1.2, IIOP and GIOP have same version numbers!
  if (this->version_.major > 1
      || this->version_.minor > 0)
    if (this->tagged_components_.decode (cdr) == 0)
      return -1;

  if (cdr.length () != 0 && TAO_debug_level)
    // If there is extra data in the profile we are supposed to
    // ignore it, but print a warning just in case...
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%d bytes out of %d left after profile data\n"),
                cdr.length (),
                encap_len));

  // Decode any additional endpoints per profile.  (At the present,
  // only RTCORBA takes advantage of this feature.)
  if (this->decode_endpoints () == -1)
    return -1;

  return 1;
}

IOP::TaggedProfile *
TAO_Profile::create_tagged_profile (void)
{
  if (this->tagged_profile_ == 0)
    {
      ACE_NEW_RETURN (this->tagged_profile_,
                      IOP::TaggedProfile,
                      0);

      // As we have not created we will now create the TaggedProfile
      this->tagged_profile_->tag = this->tag_;

      // Create the encapsulation....
      TAO_OutputCDR encap (ACE_DEFAULT_CDR_BUFSIZE,
                           TAO_ENCAP_BYTE_ORDER,
                           this->orb_core ()->output_cdr_buffer_allocator (),
                           this->orb_core ()->output_cdr_dblock_allocator (),
                           this->orb_core ()->output_cdr_msgblock_allocator (),
                           this->orb_core ()->orb_params ()->cdr_memcpy_tradeoff (),
                           TAO_DEF_GIOP_MAJOR,
                           TAO_DEF_GIOP_MINOR);

      // Create the profile body
      this->create_profile_body (encap);

      CORBA::ULong length =
        ACE_static_cast(CORBA::ULong,encap.total_length ());

#if (TAO_NO_COPY_OCTET_SEQUENCES == 1)
      // Place the message block in to the Sequence of Octets that we
      // have
      this->tagged_profile_->profile_data.replace (length,
                                                   encap.begin ());
#else
      this->tagged_profile_->profile_data.length (length);
      CORBA::Octet *buffer =
        this->tagged_profile_.profile_data.get_buffer ();
      for (const ACE_Message_Block *i = encap.begin ();
           i != encap.end ();
           i = i->next ())
        {
          ACE_OS::memcpy (buffer, i->rd_ptr (), i->length ());
          buffer += i->length ();
        }
#endif /* TAO_NO_COPY_OCTET_SEQUENCES == 1 */
    }

  return this->tagged_profile_;
}

void
TAO_Profile::set_tagged_components (TAO_OutputCDR &out_cdr)
{
  CORBA::ULong length = out_cdr.total_length ();

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = TAO_TAG_ENDPOINTS;
  tagged_component.component_data.length (length);
  CORBA::Octet *buf =
    tagged_component.component_data.get_buffer ();

  for (const ACE_Message_Block *iterator = out_cdr.begin ();
       iterator != 0;
       iterator = iterator->cont ())
    {
      CORBA::ULong i_length = iterator->length ();
      ACE_OS::memcpy (buf, iterator->rd_ptr (), i_length);

      buf += i_length;
    }

  // Add component with encoded endpoint data to this profile's
  // TaggedComponents.
  tagged_components_.set_component (tagged_component);
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

  size_t length;
  CORBA::Octet *buf = 0;

  policy_value_seq.length (policy_list->length ());

  // This loop iterates through CORBA::PolicyList to convert
  // each CORBA::Policy into a CORBA::PolicyValue
  const size_t plen = policy_list->length ();
  for (CORBA::ULong i = 0; i < plen; ++i)
    {
      TAO_OutputCDR out_CDR;
      policy_value_seq[i].ptype =
        (*policy_list)[i]->policy_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      out_CDR << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
      (*policy_list)[i]->_tao_encode (out_CDR);

      length = out_CDR.total_length ();
      policy_value_seq[i].pvalue.length (ACE_static_cast (CORBA::ULong,
                                                          length));

      buf = policy_value_seq[i].pvalue.get_buffer ();

      // Copy the CDR buffer data into the octet sequence buffer.

      for (const ACE_Message_Block *iterator = out_CDR.begin ();
           iterator != 0;
           iterator = iterator->cont ())
        {
          ACE_OS::memcpy (buf, iterator->rd_ptr (), iterator->length ());
          buf += iterator->length ();
        }
    }

  TAO_OutputCDR out_cdr;
  // Now we have to embed the Messaging::PolicyValueSeq into
  // a TaggedComponent.

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = Messaging::TAG_POLICIES;

  out_cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
  out_cdr << policy_value_seq;

  length = out_cdr.total_length ();

  tagged_component.component_data.length (ACE_static_cast (CORBA::ULong,
                                                           length));
  buf = tagged_component.component_data.get_buffer ();

  for (const ACE_Message_Block *iterator = out_cdr.begin ();
       iterator != 0;
       iterator = iterator->cont ())
    {
      size_t i_length = iterator->length ();
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


CORBA::PolicyList &
TAO_Profile::policies (ACE_ENV_SINGLE_ARG_DECL)
{
#if (TAO_HAS_CORBA_MESSAGING == 1)

  CORBA::PolicyList * pl = this->stub_->base_profiles ().policy_list_;

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

          TAO_InputCDR in_cdr (ACE_reinterpret_cast (const char *, buf),
                               tagged_component.component_data.length ());

          // Extract the Byte Order
          CORBA::Boolean byte_order;
          if ((in_cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
              return *(stub_->base_profiles ().policy_list_);
          in_cdr.reset_byte_order (ACE_static_cast(int, byte_order));

          // Now we take out the Messaging::PolicyValueSeq out from the
          // CDR.
          Messaging::PolicyValueSeq policy_value_seq;
          if (!(in_cdr >> policy_value_seq))
            {
              ACE_THROW_RETURN (CORBA::INV_OBJREF (),
                                *(stub_->base_profiles ().policy_list_));
            }

          // Here we extract the Messaging::PolicyValue out of the sequence
          // and we convert those into the proper CORBA::Policy

          CORBA::Policy_var policy;
          CORBA::ULong length = policy_value_seq.length ();

          // Set the policy list length.
          pl->length (length);

          for (CORBA::ULong i = 0; i < length; ++i)
            {
              ACE_TRY_NEW_ENV
                {
                  policy =
                    this->orb_core_->orb ()->_create_policy (
                      policy_value_seq[i].ptype
                      ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;

                  if (!CORBA::is_nil (policy.in ()))
                    {
                      buf = policy_value_seq[i].pvalue.get_buffer ();

                      TAO_InputCDR in_cdr (
                        ACE_reinterpret_cast (const char*, buf),
                        policy_value_seq[i].pvalue.length ());


                      if (!(in_cdr >> ACE_InputCDR::to_boolean (byte_order)))
                        ACE_TRY_THROW (CORBA::INV_OBJREF ());

                      in_cdr.reset_byte_order (ACE_static_cast (int,
                                                                byte_order));

                      policy->_tao_decode (in_cdr);
                      (*pl)[i] = policy._retn ();
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
                                    ACE_TEXT ("The IOR contains unsupported "
                                              "policies.\n")));
                    }
                }
              ACE_CATCHANY
                {
                  // This case should occur when in the IOR are
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

#else
  ACE_ENV_ARG_NOT_USED;    // FUZZ: ignore check_for_ace_check
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
                   CORBA::SystemException::_tao_minor_code (
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
                   CORBA::SystemException::_tao_minor_code (
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
             CORBA::SystemException::_tao_minor_code (
               TAO_DEFAULT_MINOR_CODE,
               EINVAL),
             CORBA::COMPLETED_NO));
    }
}

void
TAO_Profile::parse_string (const char *ior
                           ACE_ENV_ARG_DECL)
{
  if (!ior || !*ior)
    {
      ACE_THROW (CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                   CORBA::COMPLETED_NO));
    }

  // Remove the "N.n@" version prefix, if it exists, and verify the
  // version is one that we accept.

  // Check for version
  if (isdigit (ior [0]) &&
      ior[1] == '.' &&
      isdigit (ior [2]) &&
      ior[3] == '@')
    {
      // @@ This may fail for non-ascii character sets [but take that
      // with a grain of salt]
      this->version_.set_version ((char) (ior[0] - '0'),
                                  (char) (ior[2] - '0'));
      ior += 4;
      // Skip over the "N.n@"
    }
  else
    {
      // CORBA spec requires 1.0 if a version isn't specified.
      this->version_.set_version (1, 0);
    }

  if (this->version_.major != TAO_DEF_GIOP_MAJOR ||
      this->version_.minor >  TAO_DEF_GIOP_MINOR)
    {
      ACE_THROW (CORBA::INV_OBJREF (
                   CORBA::SystemException::_tao_minor_code (
                     TAO_DEFAULT_MINOR_CODE,
                     EINVAL),
                   CORBA::COMPLETED_NO));
    }

  this->parse_string_i (ior
                        ACE_ENV_ARG_PARAMETER);
}

// ****************************************************************

TAO_Unknown_Profile::TAO_Unknown_Profile (CORBA::ULong tag,
                                          TAO_ORB_Core *orb_core)
  : TAO_Profile (tag,
                 orb_core,
                 TAO_GIOP_Message_Version (TAO_DEF_GIOP_MAJOR,
                                           TAO_DEF_GIOP_MINOR))
{
}

TAO_Endpoint*
TAO_Unknown_Profile::endpoint (void)
{
  return 0;
}

CORBA::ULong
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

void
TAO_Unknown_Profile::parse_string_i (const char *
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
TAO_Unknown_Profile::decode_profile (TAO_InputCDR &)
{
  return 0;
}

int
TAO_Unknown_Profile::decode_endpoints (void)
{
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

const TAO::ObjectKey &
TAO_Unknown_Profile::object_key (void) const
{
  // @@ TODO this is wrong, but the function is deprecated anyway....
  static TAO::ObjectKey empty_key;
  return empty_key;
}

TAO::ObjectKey *
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

void
TAO_Unknown_Profile::create_profile_body (TAO_OutputCDR &) const
{
  // No idea about the profile body! Just return
  return;
}
