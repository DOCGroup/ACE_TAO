// -*- C++ -*-
ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Accessor to the number of bytes in the buffer.

ACE_INLINE size_t
ACE_Read_Buffer::size () const
{
  ACE_TRACE ("ACE_Read_Buffer::size");
  return this->size_;
}

// The total number of characters replaced.

ACE_INLINE size_t
ACE_Read_Buffer::replaced () const
{
  ACE_TRACE ("ACE_Read_Buffer::replaced");
  return this->occurrences_;
}

ACE_INLINE ACE_Allocator *
ACE_Read_Buffer::alloc () const
{
  ACE_TRACE ("ACE_Read_Buffer::alloc");
  return this->allocator_;
}

ACE_END_VERSIONED_NAMESPACE_DECL
