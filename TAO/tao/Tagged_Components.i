// $Id$

ACE_INLINE
TAO_Tagged_Components::TAO_Tagged_Components (void)
  :  orb_type_ (0),
     tao_priority_min_ (0),
     tao_priority_max_ (0),
     orb_type_set_ (0),
     code_sets_set_ (0),
     tao_priority_range_set_ (0)
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
TAO_Tagged_Components::get_tao_priority_range (CORBA::Short& min_p,
                                               CORBA::Short& max_p) const
{
  if (this->tao_priority_range_set_ == 1)
    {
      min_p = this->tao_priority_min_;
      max_p = this->tao_priority_max_;
    }
  return this->tao_priority_range_set_;
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
          || tag == TAO_TAG_PRIORITY_RANGE);
}

ACE_INLINE int
TAO_Tagged_Components::unique_tag (IOP::ComponentId tag) const
{
  return (tag == IOP::TAG_ORB_TYPE
          || tag == IOP::TAG_CODE_SETS
          || tag == TAO_TAG_PRIORITY_RANGE
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
          || tag == IOP::TAG_DCE_STRING_BINDING
          || tag == IOP::TAG_DCE_BINDING_NAME
          || tag == IOP::TAG_DCE_NO_PIPES
          || tag == IOP::TAG_DCE_SEC_MECH);
}
