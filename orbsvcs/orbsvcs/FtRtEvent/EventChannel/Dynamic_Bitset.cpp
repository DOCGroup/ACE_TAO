// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/Dynamic_Bitset.h"
#include <assert.h>
#include <algorithm>

#if !defined (__ACE_INLINE__)
#include "orbsvcs/FtRtEvent/EventChannel/Dynamic_Bitset.inl"
#endif /* __ACE_INLINE__ */

//FUZZ: disable check_for_lack_ACE_OS/
inline unsigned ceil(unsigned numerator, unsigned denominator)
{
  return numerator/denominator+ (numerator%denominator ? 1 : 0);
}
//FUZZ: enable check_for_lack_ACE_OS/

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

//FUZZ: disable check_for_lack_ACE_OS/
Dynamic_Bitset::Dynamic_Bitset(Dynamic_Bitset::size_type size)
  : buffer_size_(ceil(size,BITS_PER_BLOCK))
  , bit_size_(size)
  , buffer_(new block[buffer_size_])
{
//FUZZ: enable check_for_lack_ACE_OS/
  ACE_OS::memset(buffer_, 0, buffer_size_*BYTES_PER_BLOCK);
}


Dynamic_Bitset& Dynamic_Bitset::operator = (const Dynamic_Bitset& other)
{
  Dynamic_Bitset temp(other);
  // swap buffer
  block* tmp_blk = temp.buffer_;
  temp.buffer_ = this->buffer_;
  this->buffer_= tmp_blk;

  this->buffer_size_ = other.buffer_size_;
  this->bit_size_ = other.bit_size_;
  return *this;
}

Dynamic_Bitset& Dynamic_Bitset::operator &=(const Dynamic_Bitset& other)
{
  assert(other.bit_size_ == this->bit_size_);

  //FUZZ: disable check_for_lack_ACE_OS/
  size_type len = ceil(bit_size_,BITS_PER_BLOCK);
  //FUZZ: enable check_for_lack_ACE_OS/

  for (size_type i = 0; i < len; ++i)
    buffer_[i] &= other.buffer_[i];
  return *this;
}

bool Dynamic_Bitset::test(Dynamic_Bitset::size_type bit) const
{
  size_type block_pos = bit/BITS_PER_BLOCK;
  size_type bit_pos = bit%BITS_PER_BLOCK;
  block mask = 1 << bit_pos;
  return (buffer_[block_pos] & mask) !=0;
}

void Dynamic_Bitset::set(Dynamic_Bitset::size_type bit, bool val)
{
  size_type block_pos = bit/BITS_PER_BLOCK;
  size_type bit_pos = bit%BITS_PER_BLOCK;
  block mask = 1 << bit_pos;
  if (val == false)
    mask ^= static_cast<block> (-1);
  buffer_[block_pos] |= mask;
}

void Dynamic_Bitset::flip()
{
  //FUZZ: disable check_for_lack_ACE_OS/
  size_type len = ceil(bit_size_,BITS_PER_BLOCK);
  //FUZZ: enable check_for_lack_ACE_OS/

  block mask = static_cast<block> (-1);
  for (size_type i = 0; i < len; ++i)
    buffer_[i] ^= mask;
}

void Dynamic_Bitset::resize(Dynamic_Bitset::size_type num_bits, bool value)
{
  //FUZZ: disable check_for_lack_ACE_OS/
  size_type len = ceil(num_bits, BITS_PER_BLOCK);
  //FUZZ: enable check_for_lack_ACE_OS/

  if (len > this->buffer_size_) {
    Dynamic_Bitset tmp(num_bits);
    ACE_OS::memcpy(tmp.buffer_, this->buffer_, this->buffer_size_*BYTES_PER_BLOCK);
    block mask = static_cast<block> (-1);

    size_type block_pos = this->bit_size_/BITS_PER_BLOCK;
    size_type bit_pos = this->bit_size_%BITS_PER_BLOCK;

    if (value) {
      mask <<= bit_pos;
      tmp.buffer_[block_pos] |= mask;
    }
    else {
      mask >>= (BITS_PER_BLOCK-bit_pos);
      tmp.buffer_[block_pos] &= mask;
    }


    for (size_type i = block_pos +1; i < len; ++i) {
      mask = value ? static_cast<block> (-1) : 0;
        tmp.buffer_[i] = mask;
    }

    std::swap(tmp.buffer_, this->buffer_);
    this->buffer_size_ = tmp.buffer_size_;
    this->bit_size_ = tmp.bit_size_;
  }
  else
    bit_size_ = num_bits;
}

bool operator == (const Dynamic_Bitset& lhs, const Dynamic_Bitset& rhs)
{
  Dynamic_Bitset::size_type bit_size = lhs.size();
  if (bit_size != rhs.size())
    return false;
  Dynamic_Bitset::size_type block_pos = bit_size/Dynamic_Bitset::BITS_PER_BLOCK;
  Dynamic_Bitset::size_type bit_pos = bit_size%Dynamic_Bitset::BITS_PER_BLOCK;
  Dynamic_Bitset::size_type i;
  for (i = 0; i < block_pos; ++i)
    if (lhs.buffer_[i] != rhs.buffer_[i])
      return false;
  Dynamic_Bitset::block mask = static_cast<Dynamic_Bitset::block> (-1);
  mask >>= (Dynamic_Bitset::BITS_PER_BLOCK-bit_pos);
  return ((lhs.buffer_[i] ^ rhs.buffer_[i]) & mask ) == 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
