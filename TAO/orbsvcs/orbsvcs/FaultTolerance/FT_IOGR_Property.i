// -*- C++ -*-
//
//$Id$

ACE_INLINE
TAO_FT_IOGR_Property::TAO_FT_IOGR_Property (
    FT::TagFTGroupTaggedComponent &ft_group)
  :ft_group_tagged_component_ (ft_group)
{
}

ACE_INLINE
TAO_FT_IOGR_Property::~TAO_FT_IOGR_Property (void)
{
}

ACE_INLINE CORBA::Boolean
TAO_FT_IOGR_Property::reset_tagged_components (
    FT::TagFTGroupTaggedComponent &ft_group)
{
  this->ft_group_tagged_component_ =
    ft_group;

  return 1;
}
