/* -*- C++ -*- */
// $Id$

// ATM_Params.i

ACE_INLINE
ACE_ATM_Params::ACE_ATM_Params (int rw_flag,
                                const char device[],
                                Param_Info *info,
                                Param_Udata *udata,
                                int oflag,
                                int protocol_family,
                                int protocol,
                                int type,
                                int reuse_addr)
  : protocol_family_(protocol_family),
    protocol_(protocol),
    type_(type),
    reuse_addr_(reuse_addr),
    device_(device),
    info_(info),
    rw_flag_(rw_flag),
    udata_(udata),
    oflag_(oflag)
{
  ACE_TRACE ("ACE_ATM_Params::ACE_ATM_Params");
}

// Default dtor.
ACE_INLINE
ACE_ATM_Params::~ACE_ATM_Params (void)
{
  ACE_TRACE ("ACE_ATM_Params::~ACE_ATM_Params");
}

ACE_INLINE
int
ACE_ATM_Params::get_protocol_family (void) const
{
  ACE_TRACE ("ACE_ATM_Params::get_protocol_family");
  return protocol_family_;
}

ACE_INLINE
void
ACE_ATM_Params::set_protocol_family (int family)
{
  ACE_TRACE ("ACE_ATM_Params::set_protocol_family");
  protocol_family_ = family;
}

ACE_INLINE
int
ACE_ATM_Params::get_protocol (void) const
{
  ACE_TRACE ("ACE_ATM_Params::get_protocol");
  return protocol_;
}

ACE_INLINE
void
ACE_ATM_Params::set_protocol (int protocol)
{
  ACE_TRACE ("ACE_ATM_Params::set_protocol");
  protocol_ = protocol;
}

ACE_INLINE
int
ACE_ATM_Params::get_type (void) const
{
  ACE_TRACE ("ACE_ATM_Params::get_type");
  return type_;
}

ACE_INLINE
void
ACE_ATM_Params::set_type (int type)
{
  ACE_TRACE ("ACE_ATM_Params::set_type");
  type_ = type;
}

ACE_INLINE
int
ACE_ATM_Params::get_reuse_addr (void) const
{
  ACE_TRACE ("ACE_ATM_Params::get_reuse_addr");
  return reuse_addr_;
}

ACE_INLINE
void
ACE_ATM_Params::set_reuse_addr (int reuse_addr)
{
  ACE_TRACE ("ACE_ATM_Params::set_reuse_addr");
  reuse_addr_ = reuse_addr;
}

ACE_INLINE
const char*
ACE_ATM_Params::get_device (void) const
{
  ACE_TRACE ("ACE_ATM_Params::get_device");
  return device_;
}

ACE_INLINE
Param_Info*
ACE_ATM_Params::get_info (void) const
{
  ACE_TRACE ("ACE_ATM_Params::get_info");
  return info_;
}

ACE_INLINE
void
ACE_ATM_Params::set_info (Param_Info* info)
{
  ACE_TRACE ("ACE_ATM_Params::set_info");
  info_ = info;
}

ACE_INLINE
int
ACE_ATM_Params::get_rw_flag (void) const
{
  ACE_TRACE ("ACE_ATM_Params::get_rw_flag");
  return rw_flag_;
}

ACE_INLINE
void
ACE_ATM_Params::set_rw_flag (int rw_flag)
{
  ACE_TRACE ("ACE_ATM_Params::set_rw_flag");
  rw_flag_ = rw_flag;
}

ACE_INLINE
Param_Udata*
ACE_ATM_Params::get_user_data (void) const
{
  ACE_TRACE ("ACE_ATM_Params::get_user_data");
  return udata_;
}

ACE_INLINE
void
ACE_ATM_Params::set_user_data (Param_Udata *udata)
{
  ACE_TRACE ("ACE_ATM_Params::set_user_data");
  udata_ = udata;
}

ACE_INLINE
int
ACE_ATM_Params::get_oflag (void) const
{
  ACE_TRACE ("ACE_ATM_Params::get_oflag");
  return oflag_;
}

ACE_INLINE
void
ACE_ATM_Params::set_oflag (int oflag)
{
  ACE_TRACE ("ACE_ATM_Params::set_oflag");
  oflag_ = oflag;
}
