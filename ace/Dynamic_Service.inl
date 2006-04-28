// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (ACE_USES_WCHAR)

template <class TYPE> ACE_INLINE TYPE *
ACE_Dynamic_Service<TYPE>::instance (const ACE_ANTI_TCHAR *name)
{
  return instance (ACE_TEXT_TO_TCHAR_IN (name));
}

template <class TYPE> ACE_INLINE TYPE *
ACE_Dynamic_Service<TYPE>::instance (const ACE_Service_Gestalt* repo,
                                     const ACE_ANTI_TCHAR *name)
{
  return instance (repo, ACE_TEXT_CHAR_TO_TCHAR (name));
}


#endif  // ACE_USES_WCHAR

ACE_END_VERSIONED_NAMESPACE_DECL
