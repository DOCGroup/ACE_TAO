// $Id$

// ****************************************************************

ACE_INLINE
ACE_URL_Addr::ACE_URL_Addr (const ACE_URL_Addr& address)
  :  url_ (address.url_ == 0 ? 0 : ACE_OS::strdup (address.url_))
{
}

ACE_INLINE ACE_URL_Addr&
ACE_URL_Addr::operator= (const ACE_URL_Addr& address)
{
  if (this == &address)
    return *this;

  this->set (address);
  return *this;
}

ACE_INLINE LPCTSTR
ACE_URL_Addr::get_url (void) const
{
  return this->url_;
}

ACE_INLINE void
ACE_URL_Addr::set_url (LPTSTR url)
{
  this->url_ = url;
}

ACE_INLINE u_long 
ACE_URL_Addr::hash (void) const
{
  return ACE::hash_pjw (this->url_);
}

// ****************************************************************

ACE_INLINE ACE_HTTP_Addr&
ACE_HTTP_Addr::operator= (const ACE_HTTP_Addr& rhs)
{
  if (this == &rhs)
    return *this;

  this->set (rhs);
  return *this;
}

ACE_INLINE ACE_INET_Addr
ACE_HTTP_Addr::get_inet_address (void) const
{
  return ACE_INET_Addr (this->port_number_, this->hostname_);
}

ACE_INLINE LPCTSTR
ACE_HTTP_Addr::get_hostname (void) const
{
  return this->hostname_;
}

ACE_INLINE u_short
ACE_HTTP_Addr::get_port_number (void) const
{
  return this->port_number_;
}

ACE_INLINE LPCTSTR
ACE_HTTP_Addr::get_path (void) const
{
  return this->path_;
}

ACE_INLINE LPCTSTR
ACE_HTTP_Addr::get_query (void) const
{
  return this->query_;
}

// ****************************************************************

ACE_INLINE ACE_FTP_Addr&
ACE_FTP_Addr::operator= (const ACE_FTP_Addr& rhs)
{
  if (this == &rhs)
    return *this;

  this->set (rhs);
  return *this;
}

ACE_INLINE LPCTSTR
ACE_FTP_Addr::get_user (void) const
{
  return this->user_;
}

ACE_INLINE LPCTSTR
ACE_FTP_Addr::get_hostname (void) const
{
  return this->hostname_;
}

ACE_INLINE LPCTSTR
ACE_FTP_Addr::get_passwd (void) const
{
  return this->passwd_;
}

ACE_INLINE LPCTSTR
ACE_FTP_Addr::get_path (void) const
{
  return this->path_;
}

ACE_INLINE ACE_INET_Addr
ACE_FTP_Addr::get_inet_address (void) const
{
  return ACE_INET_Addr (ASYS_TEXT ("ftp"), this->hostname_);
}

// ****************************************************************

ACE_INLINE ACE_Mailto_Addr&
ACE_Mailto_Addr::operator= (const ACE_Mailto_Addr& rhs)
{
  if (this == &rhs)
    return *this;

  this->set (rhs);
  return *this;
}

ACE_INLINE LPCTSTR
ACE_Mailto_Addr::get_user (void) const
{
  return this->user_;
}

ACE_INLINE LPCTSTR
ACE_Mailto_Addr::get_hostname (void) const
{
  return this->hostname_;
}

ACE_INLINE LPCTSTR
ACE_Mailto_Addr::get_headers (void) const
{
  return this->headers_;
}
