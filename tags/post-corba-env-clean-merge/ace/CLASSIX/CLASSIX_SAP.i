/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_CLASSIX_SAP::ACE_CLASSIX_SAP(void)
    :local_addr_ (),
     enabled_    (0)
{
}

ACE_INLINE
ACE_CLASSIX_SAP::ACE_CLASSIX_SAP(const ACE_Addr& theAddr)
    :local_addr_ (theAddr.get_addr(), theAddr.get_size()),
     enabled_    (0)
{
}

ACE_INLINE
ACE_CLASSIX_SAP::~ACE_CLASSIX_SAP(void)
{
    this->unselectable();
}

ACE_INLINE
const ACE_CLASSIX_Port&
ACE_CLASSIX_SAP::get_addr(void) const
{
    return this->local_addr_;
}


ACE_INLINE
ACE_HANDLE
ACE_CLASSIX_SAP::get_handle(void) const
{
    return this->local_addr_.get_handle();
}

ACE_INLINE
int
ACE_CLASSIX_SAP::is_configured(void) const
{
    return this->local_addr_.is_configured();
}

ACE_INLINE
int
ACE_CLASSIX_SAP::is_selectable(void) const
{
    return this->enabled_ == 1;
}

ACE_INLINE
void
ACE_CLASSIX_SAP::clear(void)
{
    this->local_addr_.clear();
}
