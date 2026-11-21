// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (ACE_USES_WCHAR)

template <class TYPE> ACE_INLINE TYPE *
ACE_Dynamic_Service<TYPE>::instance (ACE_ANTI_TCHAR const *name)
{
  return instance (ACE_TEXT_CHAR_TO_TCHAR (name),false);
}

template <class TYPE> ACE_INLINE TYPE *
ACE_Dynamic_Service<TYPE>::instance (ACE_ANTI_TCHAR const *name,
                                     bool no_global)
{
  return instance (ACE_TEXT_CHAR_TO_TCHAR (name),no_global);
}

template <class TYPE> ACE_INLINE TYPE *
ACE_Dynamic_Service<TYPE>::instance (ACE_Service_Gestalt const *repo,
                                     ACE_ANTI_TCHAR const *name)
{
  return instance (repo, ACE_TEXT_CHAR_TO_TCHAR (name),false);
}

template <class TYPE> ACE_INLINE TYPE *
ACE_Dynamic_Service<TYPE>::instance (ACE_Service_Gestalt const *repo,
                                     ACE_ANTI_TCHAR const *name,
                                     bool no_global)
{
  return instance (repo, ACE_TEXT_CHAR_TO_TCHAR (name),no_global);
}

#endif  // ACE_USES_WCHAR

ACE_END_VERSIONED_NAMESPACE_DECL
