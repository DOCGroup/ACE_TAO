// $Id$

#include "tao/Profile.h"
#include "tao/Object_KeyC.h"

#include "tao/MessagingC.h"
#include "tao/Policy_Factory.h"

#if !defined (__ACE_INLINE__)
#include "tao/Profile.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Profile, "$Id$")

// ****************************************************************

TAO_Profile::~TAO_Profile (void)
{
  if (policy_list_.ptr () != 0)
    {
      for (CORBA::ULong i = 0; i < policy_list_->length (); i++)
        policy_list_[i]->destroy ();
    }
}


void
TAO_Profile::policies (CORBA::PolicyList *policy_list)
{

#if (TAO_HAS_CORBA_MESSAGING == 1)

  // @@ Angelo, can we do something about ACE_ASSERT here? Assert is
  // evil.
  ACE_ASSERT (policy_list != 0);
  this->policy_list_ = policy_list;
  // @@ Angelo, address memory management: every profile takes
  // ownership of the same copy and will try to destroy.

  Messaging::PolicyValue *pv_ptr;
  Messaging::PolicyValueSeq policy_value_seq;

  TAO_OutputCDR outCDR;

  CORBA::ULong length;
  CORBA::Octet *buf = 0;

  policy_value_seq.length(policy_list_->length());

  // This loop iterates through CORBA::PolicyList to convert
  // each CORBA::Policy into a CORBA::PolicyValue
  for (size_t i = 0; i < policy_list_->length(); i++)
    {
      ACE_NEW(pv_ptr, Messaging::PolicyValue);
    pv_ptr->ptype = (policy_list_)[i]->policy_type();

    policy_list_[i]->_tao_encode(outCDR);

    length = outCDR.total_length();
    pv_ptr->pvalue.length(length);

    buf = pv_ptr->pvalue.get_buffer();

    // Now I copy the CDR buffer data into the sequence<octect> buffer.

    for (const ACE_Message_Block *iterator = outCDR.begin();
         iterator != 0;
         iterator = iterator->cont())
    {
      ACE_OS::memcpy(buf, iterator->rd_ptr(), iterator->length());
      buf += iterator->length();
    }

    policy_value_seq[i] = (*pv_ptr);

    // Reset the CDR buffer index so that the buffer can
    // be reused for the next conversion.

    outCDR.reset();
  }

  // Now we have to embedd the Messaging::PolicyValueSeq into
  // a TaggedComponent.

  IOP::TaggedComponent tagged_component;

  tagged_component.tag = Messaging::TAG_POLICIES;
  outCDR << policy_value_seq;

  buf = tagged_component.component_data.get_buffer();

  for (const ACE_Message_Block *iterator = outCDR.begin();
       iterator != 0;
       iterator = iterator->cont())
  {
    ACE_OS::memcpy(buf, iterator->rd_ptr(), iterator->length());
    buf += iterator->length();
  }

  // Eventually we add the TaggedComponent to the TAO_TaggedComponents
  // member variable.
  tagged_components_.set_component(tagged_component);
  are_policies_parsed_ = 1;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */
}


CORBA::PolicyList&
TAO_Profile::policies ()
{

#if (TAO_HAS_CORBA_MESSAGING == 1)

  if(!are_policies_parsed_)
  {
    IOP::TaggedComponent tagged_component;
    tagged_component.tag = Messaging::TAG_POLICIES;


    // This gets a component with the proper "tag" field
    // if it exists.
    if (tagged_components_.get_component(tagged_component))
    {
      const CORBA::Octet *buf = tagged_component.component_data.get_buffer();

      TAO_InputCDR inCDR(ACE_reinterpret_cast(const char*, buf),
                         tagged_component.component_data.length());

      // Now we take out the Messaging::PolicyValueSeq out from the
      // CDR.
      Messaging::PolicyValueSeq policy_value_seq;
      inCDR >> policy_value_seq;


      // Here we extract the Messaging::PolicyValue out of the sequence
      // and we convert those into the proper CORBA::Policy

      CORBA::Policy *policy = 0;
      CORBA::ULong length = policy_value_seq.length ();
      CORBA::PolicyList *plp = 0;

      _create_policy_list (length);

      this->policy_list_ = plp;

      policy_list_->length (length);

      for (CORBA::ULong i = 0; i < length; i++)
      {
        policy = Policy_Factory::create_policy(policy_value_seq[i].ptype);
        if (policy != 0)
        {
          buf = policy_value_seq[i].pvalue.get_buffer();

          TAO_InputCDR inCDR(ACE_reinterpret_cast(const char*, buf),
                             policy_value_seq[i].pvalue.length());

          policy->_tao_decode(inCDR);

          policy_list_[i] = policy;
        }
        else
        {
        // @@ Irfan and Marina, Should I through an exception???
        }
      }
    }
    else // Create an empty list
      {
        _create_policy_list (1);
        policy_list_->length(0);
      }

  }

  return policy_list_;

#else
  _create_policy_list (1);
  policy_list_->length(0);

#endif
}

void
TAO_Profile::_create_policy_list (int length)
{
  CORBA::PolicyList *plist = 0;
  ACE_NEW (plist, CORBA::PolicyList (length));
  policy_list_ = plist;
  policy_list_->length(length);
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
