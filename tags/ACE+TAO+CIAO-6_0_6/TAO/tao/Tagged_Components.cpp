// $Id$

#include "tao/Tagged_Components.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
# include "tao/Tagged_Components.inl"
#endif /* ! __ACE_INLINE__ */

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  this->set_code_sets_i (this->code_sets_.ForCharData, ci.ForCharData);
  this->set_code_sets_i (this->code_sets_.ForWcharData, ci.ForWcharData);
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
  CORBA::ULong const max = rhs.conversion_code_sets.maximum ();
  CORBA::ULong const len = rhs.conversion_code_sets.length ();
  CONV_FRAME::CodeSetId *buffer = rhs.conversion_code_sets.get_buffer (true);
  lhs.conversion_code_sets.replace (max, len, buffer, true);
}

// ****************************************************************

void
TAO_Tagged_Components::set_component_i (IOP::ComponentId tag,
                                        TAO_OutputCDR &cdr)
{
  IOP::TaggedComponent component;
  component.tag = tag;

  // Make a *copy* of the CDR stream...
  size_t length = cdr.total_length ();
  component.component_data.length (static_cast<CORBA::ULong> (length));
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
    {
      this->set_known_component_i (component);
    }

  if (this->unique_tag (component.tag))
    {
      this->set_component_i (component);
    }
  else
    {
      this->add_component_i (component);
    }
}

void
TAO_Tagged_Components::set_component (IOP::TaggedComponent& component)
{
  if (this->known_tag (component.tag))
    {
      this->set_known_component_i (component);
    }

  if (this->unique_tag (component.tag))
    {
      this->set_component_i (component);
    }
  else
    {
      this->add_component_i (component);
    }
}

void
TAO_Tagged_Components::set_known_component_i (
    const IOP::TaggedComponent& component)
{
  TAO_InputCDR cdr (reinterpret_cast<const char*> (
                      component.component_data.get_buffer ()),
                    component.component_data.length ());

  CORBA::Boolean byte_order;

  if (!(cdr >> ACE_InputCDR::to_boolean (byte_order)))
    {
      return;
    }

  cdr.reset_byte_order (static_cast<int> (byte_order));

  if (component.tag == IOP::TAG_ORB_TYPE)
    {
      CORBA::ULong orb_type;

      if (!(cdr >> orb_type))
        {
          return;
        }

      this->orb_type_ = orb_type;
      this->orb_type_set_ = 1;
    }
  else if (component.tag == IOP::TAG_CODE_SETS)
    {
      CONV_FRAME::CodeSetComponentInfo ci;

      if (!(cdr >> ci))
        {
          return;
        }

      this->set_code_sets_i (this->code_sets_.ForCharData, ci.ForCharData);
      this->set_code_sets_i (this->code_sets_.ForWcharData, ci.ForWcharData);
      this->code_sets_set_ = 1;
    }
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
TAO_Tagged_Components::remove_component (IOP::ComponentId id)
{
  if (this->known_tag (id))
    {
      return this->remove_known_component_i (id);
    }
  else
    {
      return this->remove_component_i (id);
    }
}

int
TAO_Tagged_Components::remove_known_component_i (IOP::ComponentId tag)
{
  if (tag == IOP::TAG_ORB_TYPE)
    {
      this->orb_type_ = 0;
      this->orb_type_set_ = 0;
      return 1;
    }
  else if (tag == IOP::TAG_CODE_SETS)
    {
      this->code_sets_set_ = 0;
      return 1;
    }
  else
    {
      return 0;
    }
}

int
TAO_Tagged_Components::remove_component_i (IOP::ComponentId tag)
{
  CORBA::ULong src = 0, dest = 0;
  CORBA::ULong len = this->components_.length ();

  for (;src != len;++src)
    {
      if ( tag != this->components_[src].tag)
        {
          this->components_[dest] = this->components_[src];
          ++dest;
        }
    }

  this->components_.length (dest);
  return src - dest;
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

  if (!(cdr >> this->components_))
    {
      return 0;
    }

  CORBA::ULong const l = this->components_.length ();

  for (CORBA::ULong i = 0; i != l; ++i)
    {
      const IOP::TaggedComponent &component = this->components_[i];

      if (this->known_tag (component.tag))
        {
          this->set_known_component_i (component);
        }
    }

  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
