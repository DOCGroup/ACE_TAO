// -*- C++ -*-

//=============================================================================
/**
 *  @file   Dynamic_Bitset.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef DYNAMIC_BITSET_H
#define DYNAMIC_BITSET_H

#include <memory.h>

/**
 *
 */

class Dynamic_Bitset
{
  typedef unsigned block;
  typedef unsigned size_type;
  enum {
    BYTES_PER_BLOCK = sizeof(block),
    BITS_PER_BLOCK = sizeof(block)*8
  };
public:
  class reference {
  public:
    reference(Dynamic_Bitset* bitset, size_type bit);
    reference operator = (bool val);
    operator bool () const;
  private:
    Dynamic_Bitset* bitset_;
    size_type bit_;
  };

  Dynamic_Bitset(size_type size=0);
  ~Dynamic_Bitset();
  Dynamic_Bitset(const Dynamic_Bitset& other);
  Dynamic_Bitset& operator = (const Dynamic_Bitset& other);
    Dynamic_Bitset& operator &=(const Dynamic_Bitset& other);

  reference operator[](size_type bit);

  size_type size() const;
  bool test(size_type bit) const;

  void set(size_type bit, bool val = true);
  void flip();
  void resize(size_type num_bits, bool value = false);
    friend bool operator == (const Dynamic_Bitset& lhs, const Dynamic_Bitset& rhs);
private:
  block* buffer_;
  size_type buffer_size_;
  size_type bit_size_;
};

bool operator == (const Dynamic_Bitset& lhs, const Dynamic_Bitset& rhs);

inline Dynamic_Bitset operator & (const Dynamic_Bitset& lhs, const Dynamic_Bitset& rhs)
{
  Dynamic_Bitset tmp(lhs);
  tmp &= rhs;
  return tmp;
}



inline Dynamic_Bitset::reference::reference(Dynamic_Bitset* bitset, size_type bit)
: bitset_(bitset), bit_(bit)
{
}

inline Dynamic_Bitset::reference Dynamic_Bitset::reference::operator = (bool val)
{
  bitset_->set(bit_, val);
  return *this;
}

inline Dynamic_Bitset::reference::operator bool () const
{
  return bitset_->test(bit_);
}



inline Dynamic_Bitset::~Dynamic_Bitset()
{
  delete[] buffer_;
}

inline Dynamic_Bitset::Dynamic_Bitset(const Dynamic_Bitset& other)
: buffer_(new block[other.buffer_size_])
, buffer_size_(other.buffer_size_)
, bit_size_(other.bit_size_)
{
  memcpy(buffer_, other.buffer_, buffer_size_*BYTES_PER_BLOCK);
}




inline Dynamic_Bitset::size_type Dynamic_Bitset::size() const
{
  return bit_size_;
}

inline Dynamic_Bitset::reference Dynamic_Bitset::operator[](size_type bit)
{
  return Dynamic_Bitset::reference(this, bit);
}


#endif
