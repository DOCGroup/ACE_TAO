// -*- C++ -*-

//=============================================================================
/**
 *  @file   Dynamic_Bitset.inl
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
Dynamic_Bitset::~Dynamic_Bitset()
{
  delete[] buffer_;
}

ACE_INLINE
Dynamic_Bitset::Dynamic_Bitset(const Dynamic_Bitset& other)
  : buffer_size_(other.buffer_size_)
  , bit_size_(other.bit_size_)
  , buffer_(new block[other.buffer_size_])
{
  ACE_OS::memcpy(buffer_, other.buffer_, buffer_size_*BYTES_PER_BLOCK);
}

ACE_INLINE
Dynamic_Bitset operator & (const Dynamic_Bitset& lhs, const Dynamic_Bitset& rhs)
{
  Dynamic_Bitset tmp(lhs);
  tmp &= rhs;
  return tmp;
}

ACE_INLINE
Dynamic_Bitset::reference::reference(Dynamic_Bitset* bitset, Dynamic_Bitset::reference::size_type bit)
: bitset_(bitset), bit_(bit)
{
}

ACE_INLINE
Dynamic_Bitset::reference Dynamic_Bitset::reference::operator = (bool val)
{
  bitset_->set(bit_, val);
  return *this;
}

ACE_INLINE
Dynamic_Bitset::reference::operator bool () const
{
  return bitset_->test(bit_);
}

ACE_INLINE
Dynamic_Bitset::size_type Dynamic_Bitset::size() const
{
  return bit_size_;
}

ACE_INLINE
Dynamic_Bitset::reference Dynamic_Bitset::operator[](Dynamic_Bitset::size_type bit)
{
  return Dynamic_Bitset::reference(this, bit);
}

TAO_END_VERSIONED_NAMESPACE_DECL
