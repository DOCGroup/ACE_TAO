/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_CLASSIX_Group::ACE_CLASSIX_Group()
    : ACE_CLASSIX_Addr (sizeof (ACE_CLASSIX_Group::group_addr))
{
    this->init_();
}

ACE_INLINE
ACE_CLASSIX_Group::ACE_CLASSIX_Group(void* theSrc, int theLen)
    : ACE_CLASSIX_Addr (sizeof (ACE_CLASSIX_Group::group_addr))
{
    this->set_addr(theSrc, theLen);
}

ACE_INLINE
ACE_CLASSIX_Group::ACE_CLASSIX_Group(const KnCap& thePeer)
    : ACE_CLASSIX_Addr (sizeof (ACE_CLASSIX_Group::group_addr))
{
    this->set_addr(thePeer);
}

ACE_INLINE
void *
ACE_CLASSIX_Group::get_addr(void) const
{
    return (void*) &this->addr_;
}

ACE_INLINE
const KnUniqueId&
ACE_CLASSIX_Group::get_id(void) const
{
    return this->addr_.group_.ui;
}

ACE_INLINE
int
ACE_CLASSIX_Group::is_configured(void) const
{
    return this->addr_.config_ == 1;
}

ACE_INLINE
ACE_CLASSIX_Addr::Addr_Type
ACE_CLASSIX_Group::is_type(void) const
{
    return ACE_CLASSIX_Addr::GROUP;
}

ACE_INLINE
void
ACE_CLASSIX_Group::set_config_(void)
{
    this->addr_.config_ = 1;
}

ACE_INLINE
void
ACE_CLASSIX_Group::clear_config_(void)
{
    this->addr_.config_ = 0;
}

ACE_INLINE
const KnCap&
ACE_CLASSIX_Group::get_cap(void) const
{
    return this->addr_.group_;
}

ACE_INLINE
int
ACE_CLASSIX_Group::operator ==(const ACE_CLASSIX_Group & theRhs) const
{
    return 
	this->is_configured() &&
	theRhs.is_configured() &&
	((KnCap) (this->get_cap()) == (KnCap) (theRhs.get_cap()));
}

ACE_INLINE
int
ACE_CLASSIX_Group::operator !=(const ACE_CLASSIX_Group & theRhs) const
{
    return !(*this == theRhs);
}
