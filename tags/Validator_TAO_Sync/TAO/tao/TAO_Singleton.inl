// -*- C++ -*-
// $Id$

// Default constructors.
//
// Note: don't explicitly initialize "instance_", because TYPE may not
// have a default constructor.  Let the compiler figure it out . . .

template <class TYPE, class ACE_LOCK> ACE_INLINE
TAO_Singleton<TYPE, ACE_LOCK>::TAO_Singleton (void)
{
}

template <class TYPE, class ACE_LOCK> ACE_INLINE
TAO_TSS_Singleton<TYPE, ACE_LOCK>::TAO_TSS_Singleton (void)
{
}
