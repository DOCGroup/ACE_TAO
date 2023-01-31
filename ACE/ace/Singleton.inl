// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Default constructors.
//
// Note: don't explicitly initialize "instance_", because TYPE may not
// have a default constructor.  Let the compiler figure it out . . .

template <class TYPE, class ACE_LOCK> ACE_INLINE
ACE_Singleton<TYPE, ACE_LOCK>::ACE_Singleton ()
{
}

template <class TYPE, class ACE_LOCK> ACE_INLINE
ACE_Unmanaged_Singleton<TYPE, ACE_LOCK>::ACE_Unmanaged_Singleton ()
{
}

template <class TYPE, class ACE_LOCK> ACE_INLINE
ACE_TSS_Singleton<TYPE, ACE_LOCK>::ACE_TSS_Singleton ()
{
}

template <class TYPE, class ACE_LOCK> ACE_INLINE
ACE_Unmanaged_TSS_Singleton<TYPE, ACE_LOCK>::ACE_Unmanaged_TSS_Singleton ()
{
}

template <class TYPE, class ACE_LOCK> ACE_INLINE
ACE_DLL_Singleton_T<TYPE, ACE_LOCK>::ACE_DLL_Singleton_T ()
{
}

template <class TYPE, class ACE_LOCK>
ACE_DLL_Singleton_T<TYPE, ACE_LOCK>::~ACE_DLL_Singleton_T ()
{
}

ACE_END_VERSIONED_NAMESPACE_DECL
