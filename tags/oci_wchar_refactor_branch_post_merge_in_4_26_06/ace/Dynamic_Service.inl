// -*- C++ -*-
//
// $Id$

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TYPE> ACE_INLINE TYPE *
ACE_Dynamic_Service<TYPE>::instance (const ACE_ANTI_TCHAR *name)
{
  return instance (ACE_TEXT_TO_TCHAR_IN (name));
}

ACE_END_VERSIONED_NAMESPACE_DECL
