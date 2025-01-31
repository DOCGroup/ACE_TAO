// -*- c++ -*-
#if !defined (ACE_HAS_INLINED_OSCALLS)
# undef ACE_INLINE
# define ACE_INLINE
#endif /* ACE_HAS_INLINED_OSCALLS */

ACE_INLINE int
Parse_HTTP_Request::method () const
{
  return (int) *this->method_;
}

ACE_INLINE const char *
Parse_HTTP_Request::method_str () const
{
  return (const char *) *this->method_;
}

ACE_INLINE int
Parse_HTTP_Request::major_version () const
{
  return this->major_version_;
}

ACE_INLINE int
Parse_HTTP_Request::minor_version () const
{
  return this->minor_version_;
}

ACE_INLINE const char *
Parse_HTTP_Request::version () const
{
  return this->version_ ? this->version_ : "HTTP/0.9";
}

ACE_INLINE const char *
Parse_HTTP_Request::url () const
{
  return this->url_;
}

ACE_INLINE int
Parse_HTTP_Request::error () const
{
  return this->error_;
}
