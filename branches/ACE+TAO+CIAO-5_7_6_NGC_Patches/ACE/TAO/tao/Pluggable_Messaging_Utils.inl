// -*- C++ -*-

//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Pluggable_Reply_Params_Base::TAO_Pluggable_Reply_Params_Base (void)
  : svc_ctx_ (),
    request_id_ (0),
    is_dsi_ (0),
    dsi_nvlist_align_ (0),
    argument_flag_ (false),
    service_context_ (0),
    reply_status_ (GIOP::NO_EXCEPTION),
    locate_reply_status_ (GIOP::UNKNOWN_OBJECT)
{
}

ACE_INLINE GIOP::ReplyStatusType
TAO_Pluggable_Reply_Params_Base::reply_status (void) const
{
  return this->reply_status_;
}

ACE_INLINE void
TAO_Pluggable_Reply_Params_Base::reply_status (GIOP::ReplyStatusType status)
{
  this->reply_status_ = status;
}

ACE_INLINE GIOP::LocateStatusType
TAO_Pluggable_Reply_Params_Base::locate_reply_status (void) const
{
  return this->locate_reply_status_;
}

ACE_INLINE void
TAO_Pluggable_Reply_Params_Base::locate_reply_status (GIOP::LocateStatusType status)
{
  this->locate_reply_status_ = status;
}

ACE_INLINE void
TAO_Pluggable_Reply_Params_Base::service_context_notowned (
    IOP::ServiceContextList *svc)
{
  this->service_context_ = svc;
}

ACE_INLINE IOP::ServiceContextList &
TAO_Pluggable_Reply_Params_Base::service_context_notowned (void)
{
  return *this->service_context_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
