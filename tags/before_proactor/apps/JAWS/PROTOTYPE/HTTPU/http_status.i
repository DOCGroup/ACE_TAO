// -*- c++ -*-
// $Id$

#if !defined (ACE_HAS_INLINED_OSCALLS)
# undef ACE_INLINE
# define ACE_INLINE
#endif /* ACE_HAS_INLINED_OSCALLS */

ACE_INLINE
HTTP_SCode_Node::operator int (void) const
{
  return this->code_;
}

ACE_INLINE
HTTP_SCode_Node::operator const char * (void) const
{
  return this->code_str_;
}

ACE_INLINE
HTTP_SCode::~HTTP_SCode (void)
{
}
