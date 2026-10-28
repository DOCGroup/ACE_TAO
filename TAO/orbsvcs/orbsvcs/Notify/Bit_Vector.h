// -*- C++ -*-

//=============================================================================
/**
 *  @file    Bit_Vector.h
 *
 *  This is a basic bit vector class.
 *
 *  @author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef BIT_VECTOR_H
#define BIT_VECTOR_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

#include "ace/Vector_T.h"
#include "ace/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{
/// \brief Simple bit vector.
///
/// Written to support block allocation from persistent storage.
/// Should be promoted to the ACE level to make it generally usable.
class TAO_Notify_Serv_Export Bit_Vector
{
  using BASIC_UINT_TYPE = ACE_UINT32;
  using VECTOR_TYPE = ACE_Vector<BASIC_UINT_TYPE>;
  static constexpr int BITS_PER_WORD = 32;
  static constexpr int BPW_LOG_2 = 5;
public:
  /// The constructor.
  Bit_Vector();
  /// The destructor.
  ~Bit_Vector() = default;

  /// \brief Determine if a bit at location is set.
  bool is_set(const size_t location) const;
  /// \brief Set or unset a bit at location, growing the vector as needed.
  void set_bit(const size_t location, bool set);

  /// \brief Find the first bit that is either set or unset in an O(1).
  size_t find_first_bit(bool set) const;

private:
  /// Update our first set and unset bits.
  void evaluate_firsts(const size_t location, bool set);
  /// Iterate from location to the end, finding the first bit that
  /// matches the requested set or unset value.
  size_t find_first_bit_of(const size_t location, bool set);

private:
  VECTOR_TYPE bitvec_;
  size_t size_ {};
  size_t first_set_bit_ {};
  size_t first_cleared_bit_ {};
};
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* BIT_VECTOR_H */
