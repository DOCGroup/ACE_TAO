/* -*- C++ -*- */
// $Id$

#if defined (ACE_USES_WCHAR)
template <class TYPE> ACE_INLINE TYPE *
ACE_Dynamic_Service<TYPE>::instance (const ACE_ANTI_TCHAR *name)
{
  return instance (ACE_TEXT_CHAR_TO_TCHAR (name));
}
#endif  // ACE_USES_WCHAR
