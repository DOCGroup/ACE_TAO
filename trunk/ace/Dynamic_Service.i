/* -*- C++ -*- */
// $Id$

// Dynamic_Service.i

/* static */

template <class TYPE> ACE_INLINE TYPE *
ACE_Dynamic_Service<TYPE>::instance (const ACE_TCHAR *name)
{ 
  return  ACE_reinterpret_cast(TYPE*,ACE_Dynamic_Service_Base::instance (name));
}
