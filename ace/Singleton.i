/* -*- C++ -*- */
// $Id$

// Singleton.i

// Default constructor.

template <class TYPE, class LOCK> ACE_INLINE TYPE *
ACE_Singleton<TYPE, LOCK>::ACE_Singleton (void)
  : instance_ (new TYPE) 
{
}

template <class TYPE, class LOCK> ACE_INLINE TYPE *
ACE_TSS_Singleton<TYPE, LOCK>::ACE_TSS_Singleton (void)
  : instance_ (new ACE_TSS<TYPE>) 
{
}

