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
  if (policy_list_ != 0)
    {
      for (CORBA::ULong i = 0; i < policy_list_->length (); i++)
        (*policy_list_)[i]->destroy ();
    }
  delete policy_list_;
}


// @@ Angelo, You need ACE_UNUSED_ARG to avoid warnings about unused argument
// when files are
// compiled with TAO_HAS_CORBA_MESSAGING = 0

// Marina DONE.

void
TAO_Profile::policies (CORBA::PolicyList *policy_list)
{
#if (TAO_HAS_CORBA_MESSAGING == 1)

  // @@ Angelo, can we do something about ACE_ASSERT here? Assert is
  // evil.

  // @@ Marina Assert should be used to detect Buggy client. If this method
  // will ever receive a policy_list which is null the assertion will detect
  // the "Broken Contract" and the programmer will find out immediatly
  // that the caller is doing something wrong!

  ACE_ASSERT (policy_list != 0);

  create_policy_list (policy_list->length ());

  // Make a deep copy.
  *this->policy_list_ = *policy_list;

  // @@ Angelo, address memory management: every profile takes
  // ownership of the same copy and will try to destroy.

  // @@ Marina DONE.

  Messaging::PolicyValue *pv_ptr;
  Messaging::PolicyValueSeq policy_value_seq;

  // @@ Angelo, please use underscore for parameter names.
  // @@ Marina DONE.
  
  TAO_OutputCDR out_CDR;
  
  CORBA::ULong length;
  CORBA::Octet *buf = 0;
  
  policy_value_seq.length (policy_list_->length ());
  
  // This loop iterates through CORBA::PolicyList to convert
  // each CORBA::Policy into a CORBA::PolicyValue
  for (size_t i = 0; i < policy_list_->length (); i++)
    {
      ACE_NEW (pv_ptr, Messaging::PolicyValue);
      pv_ptr->ptype = (*policy_list_)[i]->policy_type ();
      
      (*policy_list_)[i]->_tao_encode (out_CDR);
      
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

  if (!are_policies_parsed_)
    {
      IOP::TaggedComponent tagged_component;
      tagged_component.tag = Messaging::TAG_POLICIES;

      // This gets a component with the proper "tag" field
      // if it exists.
      if (this->tagged_components_.get_component (tagged_component))
        {
          const CORBA::Octet *buf =
            tagged_component.component_data.get_buffer ();

          // use underscores.
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

          create_policy_list (length);

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
                  (*policy_list_)[i] = policy;
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
      else // Create an empty list if no policies were exposed.
        {
          // @@ Angelo, something isn't right with this code.
          // create_policy_list allocates something ... should it?
          create_policy_list (0);
          // @@ Angelo, check that the list was created for real.
          
          // @@ Marina Right now I put an assertion in the 
          // <create_policy_list> method. If there is no
          // memory and no memory can be allocated shouldn't
          // we terminate?
          
        }
      
    }

#else /* (TAO_HAS_CORBA_MESSAGING == 1) */

  create_policy_list (0);

#endif /* (TAO_HAS_CORBA_MESSAGING == 1) */

  return *policy_list_;
}

void
TAO_Profile::create_policy_list (int length)
{
  // Precondition: Make sure that the policy list
  // has not already allocated.
  ACE_ASSERT (this->policy_list_ == 0);

  ACE_NEW (this->policy_list_, CORBA::PolicyList (length));
 
  // Post-Condition: Make sure that the memory get allcated
  // for real.
  ACE_ASSERT (this->policy_list_ != 0);
  
  this->policy_list_->length (length);

  // @@ Marina & Irfan I would raise an exception in this case.
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
