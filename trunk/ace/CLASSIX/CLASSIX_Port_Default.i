/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_CLASSIX_Port_Default::ACE_CLASSIX_Port_Default(void)
    : ACE_CLASSIX_Port()
{
}

ACE_INLINE
void
ACE_CLASSIX_Port_Default::set_addr(void*, int)
{
}

ACE_INLINE
ACE_CLASSIX_Port const&  
ACE_CLASSIX_Port_Default::operator =(ACE_CLASSIX_Port const&)
{
    return *this;
}

ACE_INLINE
ACE_CLASSIX_Port const&  
ACE_CLASSIX_Port_Default::operator =(ACE_Addr const&)
{
    return *this;
}
