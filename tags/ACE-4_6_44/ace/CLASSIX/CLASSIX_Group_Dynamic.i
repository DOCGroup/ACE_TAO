/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_CLASSIX_Group_Dynamic::ACE_CLASSIX_Group_Dynamic(void)
{
    if (::grpAllocate( K_DYNAMIC, &this->addr_.group_, 0) == 0)
	this->set_config_();
}

ACE_INLINE
ACE_CLASSIX_Addr::Addr_Type
ACE_CLASSIX_Group_Dynamic::is_type(void) const
{
    return ACE_CLASSIX_Addr::DYNAMIC;
}
