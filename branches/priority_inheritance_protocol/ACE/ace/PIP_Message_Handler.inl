// $Id: 

ACE_INLINE ACE_UINT32 ACE_PIP_Message_Handler::handler_id() const
{
  return handler_id_;
}

ACE_INLINE void ACE_PIP_Message_Handler::handler_id(ACE_UINT32 id)
{
  handler_id_ = id;
}

ACE_INLINE ACE_UINT32 ACE_PIP_Message_Handler::site_id() const
{
  return site_id_;
}

ACE_INLINE void ACE_PIP_Message_Handler::site_id(ACE_UINT32 id)
{
  site_id_ = id;
}

