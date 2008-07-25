// $Id: 

#include <ace/INET_Addr.h>
#include <ace/PIP_Invocation_Manager.h>

ACE_INLINE void ACE_PIP_IO_Handler::site_id(ACE_UINT32 site_id)
{
  site_id_ = site_id;
}

ACE_INLINE ACE_UINT32 ACE_PIP_IO_Handler::site_id() const
{
  return site_id_;
}

ACE_INLINE ACE_UINT32 ACE_PIP_IO_Handler::destination_site_id() const
{
   return destination_site_id_;
}

ACE_INLINE void ACE_PIP_IO_Handler::handler_id(ACE_UINT32 handler_id)
{
   handler_id_ = handler_id;
}

ACE_INLINE ACE_UINT32 ACE_PIP_IO_Handler::handler_id() const
{
  return handler_id_;
}

ACE_INLINE ACE_INET_Addr ACE_PIP_IO_Handler::remote_address() const
{
  ACE_INET_Addr addr;
  peer_.get_remote_addr(addr);
  return addr;
}
