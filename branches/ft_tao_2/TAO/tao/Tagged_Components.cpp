// $Id$

#include "tao/Tagged_Components.h"

#if (TAO_HAS_FT_CORBA == 1)
#include "tao/ORB.h"
#endif /*TAO_HAS_FT_CORBA == 1*/

#if !defined (__ACE_INLINE__)
# include "tao/Tagged_Components.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Tagged_Components, "$Id$")


void
TAO_Tagged_Components::set_orb_type (CORBA::ULong orb_type)
{
  this->orb_type_ = orb_type;
  this->orb_type_set_ = 1;

  TAO_OutputCDR cdr;
  cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
  cdr << this->orb_type_;

  this->set_component_i (IOP::TAG_ORB_TYPE, cdr);
}

void
TAO_Tagged_Components::set_code_sets (
    const CONV_FRAME::CodeSetComponentInfo &ci)
{
  this->code_sets_ = ci;
  this->code_sets_set_ = 1;

  TAO_OutputCDR cdr;
  cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
  cdr << this->code_sets_;

  this->set_component_i (IOP::TAG_CODE_SETS, cdr);
}

void
TAO_Tagged_Components::set_code_sets (CONV_FRAME::CodeSetComponentInfo &ci)
{
  this->set_code_sets_i (this->code_sets_.ForCharData,
                         ci.ForCharData);
  this->set_code_sets_i (this->code_sets_.ForWcharData,
                         ci.ForWcharData);
  this->code_sets_set_ = 1;

  TAO_OutputCDR cdr;
  cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);
  cdr << this->code_sets_;

  this->set_component_i (IOP::TAG_CODE_SETS, cdr);
}

void
TAO_Tagged_Components::set_code_sets_i (
    CONV_FRAME::CodeSetComponent &lhs,
    CONV_FRAME::CodeSetComponent &rhs)
{
  lhs.native_code_set = rhs.native_code_set;
  CORBA::ULong max = rhs.conversion_code_sets.maximum ();
  CORBA::ULong len = rhs.conversion_code_sets.length ();
  CONV_FRAME::CodeSetId *buffer = rhs.conversion_code_sets.get_buffer (1);
  lhs.conversion_code_sets.replace (max, len, buffer, 1);
}

void
TAO_Tagged_Components::set_tao_priority (CORBA::Short p)
{
  this->tao_priority_ = p;
  this->tao_priority_set_ = 1;

  TAO_OutputCDR cdr;
  cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER);

  cdr << this->tao_priority_;

  this->set_component_i (TAO_TAG_PRIORITY, cdr);
}

// ****************************************************************

void
TAO_Tagged_Components::set_component_i (IOP::ComponentId tag,
                                        TAO_OutputCDR &cdr)
{
  IOP::TaggedComponent component;
  component.tag = tag;

  // Make a *copy* of the CDR stream...
  CORBA::ULong length = cdr.total_length ();
  component.component_data.length (length);
  CORBA::Octet *buf = component.component_data.get_buffer ();

  for (const ACE_Message_Block *i = cdr.begin ();
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }

  this->set_component_i (component);
}

void
TAO_Tagged_Components::set_component (const IOP::TaggedComponent& component)
{
  if (this->known_tag (component.tag))
    this->set_known_component_i (component);
  if (this->unique_tag (component.tag))
    this->set_component_i (component);
  else
    this->add_component_i (component);
}

void
TAO_Tagged_Components::set_component (IOP::TaggedComponent& component)
{
  if (this->known_tag (component.tag))
    this->set_known_component_i (component);
  if (this->unique_tag (component.tag))
    this->set_component_i (component);
  else
    this->add_component_i (component);
}

void
TAO_Tagged_Components::set_known_component_i (
    const IOP::TaggedComponent& component)
{
  TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                          component.component_data.get_buffer ()),
                    component.component_data.length ());
  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return;
  cdr.reset_byte_order (ACE_static_cast(int,byte_order));

  if (component.tag == IOP::TAG_ORB_TYPE)
    {
      CORBA::ULong orb_type;
      if ((cdr >> orb_type) == 0)
        return;

      this->orb_type_ = orb_type;
      this->orb_type_set_ = 1;
    }
  else if (component.tag == IOP::TAG_CODE_SETS)
    {
      CONV_FRAME::CodeSetComponentInfo ci;
      if ((cdr >> ci) == 0)
        return;

      this->set_code_sets_i (this->code_sets_.ForCharData,
                             ci.ForCharData);
      this->set_code_sets_i (this->code_sets_.ForWcharData,
                             ci.ForWcharData);
      this->code_sets_set_ = 1;
    }
  else if (component.tag == TAO_TAG_PRIORITY)
    {
      CORBA::Short p;

      if ((cdr >> p) == 0)
        return;

      this->tao_priority_ = p;
      this->tao_priority_set_ = 1;
    }
}


void
TAO_Tagged_Components::set_unique_component_i (
    const IOP::TaggedComponent& component)
{
  TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                          component.component_data.get_buffer ()),
                    component.component_data.length ());

  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return;
  cdr.reset_byte_order (ACE_static_cast(int,byte_order));

# if (TAO_HAS_FT_CORBA == 1)
  if (component.tag == IOP::TAG_FT_PRIMARY)
    {
      CORBA::Boolean primary;
      if ((cdr.read_boolean (primary)) == 0)
        return;

      this->ft_tag_primary_ = 1;
    }
  else if (component.tag == IOP::TAG_FT_GROUP)
    {
      // Revision numbers of the FT specification
      CORBA::Octet ft_minor, ft_major;

      if ((cdr.read_octet (ft_major)) == 0)
        return;
      if ((cdr.read_octet (ft_minor)) == 0)
        return;

      // Check for the revsion numbers
      if (ft_major != TAO_DEF_FT_CORBA_MAJOR &&
          ft_minor != TAO_DEF_FT_CORBA_MINOR)
        {
          // @@Should we print out an error??
          return;
        }

      // Read the FTDomainId
      if (cdr.char_translator () == 0)
        {
          CORBA::ULong length = 0;
          if (cdr.read_ulong (length))
            {
              // Do not include NULL character at the end.
              // @@ This is not getting demarshaled using the codeset
              //    translators!
              this->ft_tagged_component_.ft_domain_id_.set (cdr.rd_ptr (),
                                                            length - 1,
                                                            0);
              cdr.skip_bytes (length);
            }
        }
      else
        {
          CORBA::String_var tmp;
          cdr.read_string (tmp.inout ());
          this->ft_tagged_component_.ft_domain_id_.set (tmp._retn (), 1);
        }

      // Read the Object Group ID
      CORBA::ULongLong group_id;
      if (cdr.read_ulonglong (group_id) == 0)
        return;

      this->ft_tagged_component_.object_group_id_ = group_id;

      // Read the object group ref version
      CORBA::ULong ref_version;
      if (cdr.read_ulong (ref_version) == 0)
        return;

      this->ft_tagged_component_.object_group_ref_version_ =
        ref_version;
    }
#endif /*TAO_HAS_FT_CORBA == 1 */

}

void
TAO_Tagged_Components::set_component_i (const IOP::TaggedComponent& component)
{
  // @@ TODO Some components can show up multiple times, others
  //    can't find out and take appropiate action.
  for (CORBA::ULong i = 0; i != this->components_.length (); ++i)
    {
      if (component.tag == this->components_[i].tag)
        {
          this->components_[i] = component;
          return;
        }
    }
  this->add_component_i (component);
}

void
TAO_Tagged_Components::set_component_i (IOP::TaggedComponent& component)
{
  for (CORBA::ULong i = 0; i != this->components_.length (); ++i)
    {
      if (component.tag == this->components_[i].tag)
        {
          CORBA::ULong max = component.component_data.maximum ();
          CORBA::ULong len = component.component_data.length ();
          CORBA::Octet* buf = component.component_data.get_buffer (1);
          this->components_[i].component_data.replace (max, len, buf, 1);
          return;
        }
    }
  this->add_component_i (component);
}

void
TAO_Tagged_Components::add_component_i (IOP::TaggedComponent& component)
{
  // @@ TODO Some components can show up multiple times, others
  //    can't find out and take appropiate action.
  CORBA::ULong l = this->components_.length ();
  this->components_.length (l + 1);
  this->components_[l].tag = component.tag;
  CORBA::ULong max = component.component_data.maximum ();
  CORBA::ULong len = component.component_data.length ();
  CORBA::Octet* buf = component.component_data.get_buffer (1);
  this->components_[l].component_data.replace (max, len, buf, 1);
}

void
TAO_Tagged_Components::add_component_i (const IOP::TaggedComponent& component)
{
  // @@ TODO Some components can show up multiple times, others
  //    can't find out and take appropiate action.
  CORBA::ULong l = this->components_.length ();
  this->components_.length (l + 1);
  this->components_[l] = component;
}

int
TAO_Tagged_Components::get_component (IOP::TaggedComponent& component) const
{
  for (CORBA::ULong i = 0; i != this->components_.length (); ++i)
    {
      if (component.tag == this->components_[i].tag)
        {
          component = this->components_[i];
          return 1;
        }
    }
  return 0;
}

// ****************************************************************

int
TAO_Tagged_Components::encode (TAO_OutputCDR& cdr) const
{
  return (cdr << this->components_);
}

int
TAO_Tagged_Components::decode (TAO_InputCDR& cdr)
{
  // Mark the well-known components as removed
  this->orb_type_set_ = 0;
  this->code_sets_set_ = 0;
  this->tao_priority_set_ = 0;

  if ((cdr >> this->components_) == 0)
    return 0;

  CORBA::ULong l = this->components_.length ();
  for (CORBA::ULong i = 0; i != l; ++i)
    {
      const IOP::TaggedComponent &component =
        this->components_[i];
      if (this->known_tag (component.tag))
        this->set_known_component_i (component);
      else if (this->unique_tag (component.tag))
        this->set_unique_component_i (component);
    }
  return 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Unbounded_Sequence<IOP::TaggedComponent>;
template class TAO_Unbounded_Sequence<IOP::TaggedProfile>;
template class TAO_Unbounded_Sequence<IOP::ServiceContext>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Unbounded_Sequence<IOP::TaggedComponent>
#pragma instantiate TAO_Unbounded_Sequence<IOP::TaggedProfile>
#pragma instantiate TAO_Unbounded_Sequence<IOP::ServiceContext>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
