// $Id$

ACE_INLINE
TAO_Tagged_Components::TAO_Tagged_Components (void)
  :  orb_type_ (0),
     tao_priority_ (0),
     orb_type_set_ (0),
     code_sets_set_ (0),
     tao_priority_set_ (0)
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
TAO_Tagged_Components::get_tao_priority (CORBA::Short& p) const
{
  if (this->tao_priority_set_ == 1)
    {
      p = this->tao_priority_;
    }
  return this->tao_priority_set_;
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
          || tag == IOP::TAG_CODE_SETS
          || tag == TAO_TAG_PRIORITY);
}

ACE_INLINE int
TAO_Tagged_Components::unique_tag (IOP::ComponentId tag) const
{
  return (tag == IOP::TAG_ORB_TYPE
          || tag == IOP::TAG_CODE_SETS
          || tag == TAO_TAG_PRIORITY
          || tag == IOP::TAG_POLICIES
          // || tag == IOP::TAG_ALTERNATE_IIOP_ADDRESS
          || tag == IOP::TAG_ASSOCIATION_OPTIONS
          || tag == IOP::TAG_SEC_NAME
          || tag == IOP::TAG_SPKM_1_SEC_MECH
          || tag == IOP::TAG_SPKM_2_SEC_MECH
          || tag == IOP::TAG_KerberosV5_SEC_MECH
          || tag == IOP::TAG_CSI_ECMA_Secret_SEC_MECH
          || tag == IOP::TAG_CSI_ECMA_Hybrid_SEC_MECH
          || tag == IOP::TAG_SSL_SEC_TRANS
          || tag == IOP::TAG_CSI_ECMA_Public_SEC_MECH
          || tag == IOP::TAG_GENERIC_SEC_MECH
          || tag == IOP::TAG_COMPLETE_OBJECT_KEY
          || tag == IOP::TAG_ENDPOINT_ID_POSITION
          || tag == IOP::TAG_LOCATION_POLICY
          || tag == IOP::TAG_FT_PRIMARY
          || tag == IOP::TAG_FT_GROUP
          || tag == IOP::TAG_DCE_STRING_BINDING
          || tag == IOP::TAG_DCE_BINDING_NAME
          || tag == IOP::TAG_DCE_NO_PIPES
          || tag == IOP::TAG_DCE_SEC_MECH);
}

ACE_INLINE IOP::MultipleComponentProfile&
TAO_Tagged_Components::components (void)
{
  return this->components_;
}

ACE_INLINE CORBA::Boolean 
TAO_Tagged_Components::is_primary (void)
{
  return this->ft_tag_primary_;
}

ACE_INLINE TAO_Tagged_Components::TAO_FT_Group_Tagged_Component &
TAO_Tagged_Components::ft_group_tagged_component (void)
{
  return this->ft_tagged_component_;
}

ACE_INLINE const TAO_Tagged_Components::TAO_FT_Group_Tagged_Component&
TAO_Tagged_Components::ft_group_tagged_component (void)
{
  return this->ft_tagged_component_;
}
