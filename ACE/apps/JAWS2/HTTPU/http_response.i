// -*- c++ -*-
// $Id$

#if !defined (ACE_HAS_INLINED_OSCALLS)
# undef ACE_INLINE
# define ACE_INLINE
#endif /* ACE_HAS_INLINED_OSCALLS */

ACE_INLINE
HTTP_Response::HTTP_Response (void)
{
}

ACE_INLINE
HTTP_Response::~HTTP_Response (void)
{
}

ACE_INLINE Parse_HTTP_Response *
HTTP_Response::response_line (void)
{
  return &(this->response_);
}
