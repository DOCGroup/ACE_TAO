/* -*- C++ -*- */
// $Id$

ACE_INLINE
void
ACE_CLASSIX_Port_Core::set_config_(void)
{
    this->config_ = 1;
}

ACE_INLINE
int
ACE_CLASSIX_Port_Core::is_configured(void) const
{
    return this->config_ == 1;
}


ACE_INLINE
ACE_HANDLE
ACE_CLASSIX_Port_Core::get_handle(void) const
{
    return this->addr_.handle;
}

ACE_INLINE
const KnUniqueId&
ACE_CLASSIX_Port_Core::get_id(void) const
{
    return this->addr_.id;
}

ACE_INLINE
void*
ACE_CLASSIX_Port_Core::get_addr(void) const
{
    return (void*) (&this->addr_);
}
