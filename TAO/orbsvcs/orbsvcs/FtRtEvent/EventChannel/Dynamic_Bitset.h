// -*- C++ -*-

//=============================================================================
/**
 *  @file   Dynamic_Bitset.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef DYNAMIC_BITSET_H
#define DYNAMIC_BITSET_H

#include "ace/config-all.h"

#include "tao/Versioned_Namespace.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class Dynamic_Bitset
{
  using block = unsigned;
  using size_type = unsigned;
  static constexpr size_type BYTES_PER_BLOCK = sizeof(block);
  static constexpr size_type BITS_PER_BLOCK = sizeof(block) * 8;
public:
  class reference {
  public:
    using size_type = unsigned;
    reference(Dynamic_Bitset* bitset, size_type bit);
    reference operator = (bool val);
    explicit operator bool () const;
  private:
    Dynamic_Bitset* bitset_;
    size_type bit_;
  };

  explicit Dynamic_Bitset(size_type size = 0);
  ~Dynamic_Bitset();
  Dynamic_Bitset(Dynamic_Bitset const& other);
  Dynamic_Bitset(Dynamic_Bitset&&) = delete;
  Dynamic_Bitset& operator = (Dynamic_Bitset const& other);
  Dynamic_Bitset& operator = (Dynamic_Bitset&&) = delete;
  Dynamic_Bitset& operator &=(Dynamic_Bitset const& other);

  reference operator[](size_type bit);

  size_type size() const;
  bool test(size_type bit) const;

  void set(size_type bit, bool val = true);
  void flip();
  void resize(size_type num_bits, bool value = false);

  friend bool operator == (Dynamic_Bitset const& lhs, Dynamic_Bitset const& rhs);
private:
  size_type buffer_size_;
  size_type bit_size_;
  block* buffer_;
};

bool operator == (Dynamic_Bitset const& lhs, Dynamic_Bitset const& rhs);
Dynamic_Bitset operator & (Dynamic_Bitset const& lhs, Dynamic_Bitset const& rhs);

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/FtRtEvent/EventChannel/Dynamic_Bitset.inl"
#endif /* __ACE_INLINE__ */

#endif
