/* -*- C++ -*- */
// $Id$

// CLASSIX_Addr.i

ACE_INLINE
ACE_CLASSIX_Addr::ACE_CLASSIX_Addr(int theSize)
    : ACE_Addr (AF_CLASSIX, theSize)
{
}

ACE_INLINE
ACE_CLASSIX_Addr::~ACE_CLASSIX_Addr(void)
{
}

ACE_INLINE
const KnUniqueId&
ACE_CLASSIX_Addr::get_id(void) const
{
  return ACE_CLASSIX_OS::null_KnUniqueId();
}

ACE_INLINE
ACE_HANDLE
ACE_CLASSIX_Addr::get_handle(void) const
{
  return ACE_INVALID_HANDLE;
}

ACE_INLINE
ACE_HANDLE
ACE_CLASSIX_Addr::get_port_number(void) const
{
  return get_handle();
}

ACE_INLINE
int
ACE_CLASSIX_Addr::is_configured(void) const
{
  return 0;
}

ACE_INLINE
ACE_CLASSIX_Addr::Addr_Type
ACE_CLASSIX_Addr::is_type(void) const
{
  return ACE_CLASSIX_Addr::UNDEFINED;
}

ACE_INLINE
void
ACE_CLASSIX_Addr::dump(void) const
{
}
