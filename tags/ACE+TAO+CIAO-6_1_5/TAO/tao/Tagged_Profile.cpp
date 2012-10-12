// $Id$

#include "tao/Tagged_Profile.h"
#include "tao/ORB_Core.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Transport_Acceptor.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/debug.h"
#include "tao/target_specification.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
# include "tao/Tagged_Profile.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Boolean
TAO_Tagged_Profile::extract_object_key (IOP::TaggedProfile &profile)
{
  // Get our Acceptor registry
  TAO_Acceptor_Registry &acceptor_registry =
    this->orb_core_->lane_resources ().acceptor_registry ();

  // Get the right acceptor for the tag in the TaggedProfile
  TAO_Acceptor *acceptor = acceptor_registry.get_acceptor (profile.tag);

  if (acceptor)
    {
      // Get the object key
      if (acceptor->object_key (profile, this->object_key_) == -1)
        {
          return false;
        }
    }
  else
    {
      if (TAO_debug_level)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t)TAO_Tagged_Profile\n")));
        }

      return false;
    }

  return true;
}

CORBA::Boolean
TAO_Tagged_Profile::unmarshall_target_address (TAO_InputCDR &cdr)
{
  CORBA::Boolean hdr_status = cdr.read_short (this->discriminator_);

  if (hdr_status)
    {
      switch (this->discriminator_)
        {
        case TAO_Target_Specification::Key_Addr:
          hdr_status = this->unmarshall_object_key_i (cdr);
          break;

        case TAO_Target_Specification::Profile_Addr:
          hdr_status = this->unmarshall_iop_profile_i (cdr);
          break;

        case TAO_Target_Specification::Reference_Addr:
          hdr_status = this->unmarshall_ref_addr_i (cdr);
          break;

        default:
          hdr_status = false;
          break;
        }
    }

  return hdr_status;
}

CORBA::Boolean
TAO_Tagged_Profile::unmarshall_object_key (TAO_InputCDR &input)
{
  this->discriminator_ = TAO_Target_Specification::Key_Addr;

  return this->unmarshall_object_key_i (input);
}


CORBA::Boolean
TAO_Tagged_Profile::unmarshall_object_key_i (TAO_InputCDR &input)
{
  CORBA::Boolean hdr_status = (CORBA::Boolean) input.good_bit ();

  CORBA::Long key_length = 0;
  hdr_status = hdr_status && input.read_long (key_length);

  if (hdr_status)
    {
      this->object_key_.replace (key_length,
                                 key_length,
                                 (CORBA::Octet*)input.rd_ptr (),
                                 0);
      input.skip_bytes (key_length);

      this->object_key_extracted_ = true;
    }

  return hdr_status;
}


CORBA::Boolean
TAO_Tagged_Profile::unmarshall_iop_profile_i (TAO_InputCDR &input)
{
  CORBA::Boolean hdr_status = (CORBA::Boolean) input.good_bit ();

  // Extract into the IOP::Tagged profile.
  hdr_status &= input >> this->profile_;

  return hdr_status;
}

CORBA::Boolean
TAO_Tagged_Profile::unmarshall_ref_addr_i (TAO_InputCDR &input)
{
  CORBA::Boolean hdr_status = (CORBA::Boolean) input.good_bit ();

  /*
   * The GIOP::IORAddressingInfo is defined as follows
   *   struct IORAddressingInfo
   *     {
   *       unsigned long selected_profile_index;
   *       IOP::IOR ior;
   *     };
   *
   * and the IOP::IOR is defined to be
   *   struct IOR
   *      {
   *        string type_id;
   *        sequence<TaggedProfile>   profiles;
   *      };
   */

  // First read the profile index
  CORBA::ULong prof_index =  0;

  hdr_status = hdr_status && input.read_ulong (prof_index);

  // Set the value in TAO_Tagged_Profile
  if (hdr_status)
    {
      this->profile_index_ = prof_index;
    }

  // Get the length of the type_id
  CORBA::Long id_length = 0;
  hdr_status = hdr_status && input.read_long (id_length);

  if (hdr_status)
    {
      // Set the type_id (it is not owned by this object)
      this->type_id_ = input.rd_ptr ();

      input.skip_bytes (id_length);
    }

  // Unmarshall the sequence of TaggedProfiles
  IOP::TaggedProfileSeq ior_profiles;

  hdr_status &= input >> ior_profiles;

  // Get the right TaggedProfile from the <ior_profiles>
  if (hdr_status)
    {
      this->profile_ = ior_profiles [prof_index];
    }

  return hdr_status;
}

TAO_END_VERSIONED_NAMESPACE_DECL
