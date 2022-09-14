// -*- c++ -*-
#if !defined (ACE_HAS_INLINED_OSCALLS)
# undef ACE_INLINE
# define ACE_INLINE
#endif /* ACE_HAS_INLINED_OSCALLS */

ACE_INLINE
HTTP_SCode_Node::operator int () const
{
  return this->code_;
}

ACE_INLINE
HTTP_SCode_Node::operator const char * () const
{
  return this->code_str_;
}
