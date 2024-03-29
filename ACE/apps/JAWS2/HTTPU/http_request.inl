// -*- c++ -*-
#if !defined (ACE_HAS_INLINED_OSCALLS)
# undef ACE_INLINE
# define ACE_INLINE
#endif /* ACE_HAS_INLINED_OSCALLS */

ACE_INLINE
HTTP_Request::HTTP_Request ()
{
}

ACE_INLINE
HTTP_Request::~HTTP_Request ()
{
}

ACE_INLINE Parse_HTTP_Request *
HTTP_Request::request_line ()
{
  return &(this->request_);
}

ACE_INLINE const Parse_HTTP_Request *
HTTP_Request::request_line () const
{
  return &(this->request_);
}

ACE_INLINE HTTP_Parse_URL *
HTTP_Request::url ()
{
  return &(this->url_);
}

ACE_INLINE void
HTTP_Request::set_status (int s)
{
  this->status_ = s;
}
