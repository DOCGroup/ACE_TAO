// $Id$

#include "tao/Profile.h"
#include "tao/Object_KeyC.h"

#include "tao/MessagingC.h"
#include "tao/Policy_Factory.h"
#include "tao/Stub.h"

#if !defined (__ACE_INLINE__)
#include "tao/Profile.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Profile, "$Id$")

// ****************************************************************

TAO_Profile::~TAO_Profile (void)
{
}

void
TAO_Profile::policies (CORBA::PolicyList *policy_list)
{
#if (TAO_HAS_CORBA_MESSAGING == 1)

  ACE_ASSERT (policy_list != 0);

  Messaging::PolicyValue *pv_ptr;
  Messaging::PolicyValueSeq policy_value_seq;

  TAO_OutputCDR out_CDR;

  CORBA::ULong length;
  CORBA::Octet *buf = 0;

  policy_value_seq.length (policy_list->length ());

  // This loop iterates through CORBA::PolicyList to convert
  // each CORBA::Policy into a CORBA::PolicyValue
  for (size_t i = 0; i < policy_list->length (); i++)
    {
      // @@ Angelo, avoid unnecessary memory allocations like the one below -
      // they are very expensive!
      ACE_NEW (pv_ptr, Messaging::PolicyValue);
      pv_ptr->ptype = (*policy_list)[i]->policy_type ();

      (*policy_list)[i]->_tao_encode (out_CDR);

      length = out_CDR.total_length ();
      pv_ptr->pvalue.length (length);

      buf = pv_ptr->pvalue.get_buffer ();

      // Copy the CDR buffer data into the sequence<octect> buffer.

      for (const ACE_Message_Block *iterator = out_CDR.begin ();
           iterator != 0;
           iterator = iterator->cont ())
        {
          ACE_OS::memcpy (buf, iterator->rd_ptr (), iterator->length ());
          buf += iterator->length ();
        }

      policy_value_seq[i] = *pv_ptr;
      delete pv_ptr;

      // Reset the CDR buffer index so that the buffer can
      // be reused for the next conversion.

      out_CDR.reset ();
    }

  // Now we have to embedd the Messaging::PolicyValueSeq into
  // a TaggedComponent.

  IOP::TaggedComponent tagged_component;

  tagged_component.tag = Messaging::TAG_POLICIES;
  out_CDR << policy_value_seq;

  buf = tagged_component.component_data.get_buffer ();

  for (const ACE_Message_Block *iterator = out_CDR.begin ();
       iterator != 0;
       iterator = iterator->cont ())
  {
    ACE_OS::memcpy (buf, iterator->rd_ptr (), iterator->length ());
    buf += iterator->length ();
  }

  // Eventually we add the TaggedComponent to the TAO_TaggedComponents
  // member variable.
  tagged_components_.set_component (tagged_component);
  are_policies_parsed_ = 1;

#else /* TAO_HAS_CORBA_MESSAGING == 1 */

  ACE_UNUSED_ARG (policy_list);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
}


CORBA::PolicyList&
TAO_Profile::policies (void)
{
#if (TAO_HAS_CORBA_MESSAGING == 1)

  CORBA::PolicyList *policies = stub_->base_profiles ().policy_list ();
  if (!are_policies_parsed_
      && (policies->length () == 0))
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

          TAO_InputCDR in_CDR (ACE_reinterpret_cast (const char*, buf),
                              tagged_component.component_data.length ());

          // Now we take out the Messaging::PolicyValueSeq out from the
          // CDR.
          Messaging::PolicyValueSeq policy_value_seq;
          in_CDR >> policy_value_seq;

          // Here we extract the Messaging::PolicyValue out of the sequence
          // and we convert those into the proper CORBA::Policy

          CORBA::Policy *policy = 0;
          CORBA::ULong length = policy_value_seq.length ();

          // Get the policy list from the MProfile.
          CORBA::PolicyList *policies = stub_->base_profiles ().policy_list ();
          policies->length (length);

          for (CORBA::ULong i = 0; i < length; i++)
            {
              policy =
                TAO_Policy_Factory::create_policy (policy_value_seq[i].ptype);
              if (policy != 0)
                {
                  buf = policy_value_seq[i].pvalue.get_buffer ();

                  TAO_InputCDR in_CDR (ACE_reinterpret_cast (const char*, buf),
                                       policy_value_seq[i].pvalue.length ());

                  policy->_tao_decode (in_CDR);
                  (*policies)[i] = policy;
                }
              else
                {
                  // This case should occure when in the IOR are embedded
                  // policies that TAO doesn't support, so as specified
                  // by the RT-CORBA spec. ptc/99-05-03 we just ignore
                  // this un-understood policies.
                }
            }
        }
      else
        {
          // @@ Marina, what should happen here?
        }

    }

#endif /* (TAO_HAS_CORBA_MESSAGING == 1) */

  return *(stub_->base_profiles ().policy_list ());
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

// ****************************************************************

TAO_Unknown_Profile::TAO_Unknown_Profile (CORBA::ULong tag)
  : TAO_Profile (tag),
    tagged_profile_ ()
{
}

int
TAO_Unknown_Profile::parse_string (const char *,
                                   CORBA::Environment &)
{
  // @@ THROW something????
  return -1;
}

char
TAO_Unknown_Profile::object_key_delimiter (void) const
{
  return 0;
}

char *
TAO_Unknown_Profile::to_string (CORBA::Environment &)
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
TAO_Unknown_Profile::hash (CORBA::ULong max,
                           CORBA::Environment &)
{
  return (ACE::hash_pjw (ACE_reinterpret_cast (const char*,
                                               this->body_.get_buffer ()),
                         this->body_.length ()) % max);
}

int
TAO_Unknown_Profile::addr_to_string (char * /* buffer */,
                                     size_t /* length */)
{
  return -1;
}

void
TAO_Unknown_Profile::reset_hint (void)
{
  // do nothing
}

IOP::TaggedProfile&
TAO_Unknown_Profile::create_tagged_profile (void)
{
  this->tagged_profile_.tag = this->tag ();

  // I dont know about the rest, so we return our copy
  return this->tagged_profile_;

}
