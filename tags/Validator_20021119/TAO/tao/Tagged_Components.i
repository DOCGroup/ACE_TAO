// -*- C++ -*-
//
// $Id$

ACE_INLINE
TAO_Tagged_Components::TAO_Tagged_Components (void)
  :  orb_type_ (0),
     orb_type_set_ (0),
     code_sets_set_ (0)
{
}

ACE_INLINE int
TAO_Tagged_Components::get_orb_type (CORBA::ULong& orb_type) const
{
  if (this->orb_type_set_ == 1)
    orb_type = this->orb_type_;
  return this->orb_type_set_;
}

ACE_INLINE int
TAO_Tagged_Components::get_code_sets (const CONV_FRAME::CodeSetComponentInfo* &ci) const
{
  if (this->code_sets_set_ == 1)
    ci = &this->code_sets_;
  return this->code_sets_set_;
}

ACE_INLINE int
TAO_Tagged_Components::get_code_sets (CONV_FRAME::CodeSetComponentInfo &ci) const
{
  if (this->code_sets_set_ == 1)
    ci = this->code_sets_;
  return this->code_sets_set_;
}

ACE_INLINE int
TAO_Tagged_Components::known_tag (IOP::ComponentId tag) const
{
  return (tag == IOP::TAG_ORB_TYPE
          || tag == IOP::TAG_CODE_SETS);
}


ACE_INLINE int
TAO_Tagged_Components::unique_tag (IOP::ComponentId tag) const
{
  return (tag == IOP::TAG_ORB_TYPE
          || tag == IOP::TAG_CODE_SETS
          || tag == IOP::TAG_POLICIES
          || tag == TAO_TAG_ENDPOINTS
          // || tag == IOP::TAG_ALTERNATE_IIOP_ADDRESS
          || tag == IOP::TAG_COMPLETE_OBJECT_KEY
          || tag == IOP::TAG_ENDPOINT_ID_POSITION
          || tag == IOP::TAG_LOCATION_POLICY
          || tag == IOP::TAG_FT_PRIMARY
          || tag == IOP::TAG_FT_GROUP
          || tag == IOP::TAG_DCE_STRING_BINDING
          || tag == IOP::TAG_DCE_BINDING_NAME
          || tag == IOP::TAG_DCE_NO_PIPES);
}

ACE_INLINE IOP::MultipleComponentProfile&
TAO_Tagged_Components::components (void)
{
  return this->components_;
}
