// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE_Mem_Map const &
ACE_MMAP_Memory_Pool::mmap () const
{
  return mmap_;
}

ACE_INLINE
ACE_Mem_Map &
ACE_MMAP_Memory_Pool::mmap ()
{
  return mmap_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
