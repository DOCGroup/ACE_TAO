// -*- c++ -*-
#if !defined (ACE_HAS_INLINED_OSCALLS)
# undef ACE_INLINE
# define ACE_INLINE
#endif /* ACE_HAS_INLINED_OSCALLS */

ACE_INLINE int
Parse_HTTP_Response::code () const
{
  return this->code_;
}

ACE_INLINE const char *
Parse_HTTP_Response::code_str () const
{
  return this->code_str_ ? this->code_str_ : "200";
}

ACE_INLINE int
Parse_HTTP_Response::major_version () const
{
  return this->major_version_;
}

ACE_INLINE int
Parse_HTTP_Response::minor_version () const
{
  return this->minor_version_;
}

ACE_INLINE const char *
Parse_HTTP_Response::version () const
{
  return this->version_ ? this->version_ : "HTTP/0.9";
}

ACE_INLINE int
Parse_HTTP_Response::error () const
{
  return this->error_;
}
