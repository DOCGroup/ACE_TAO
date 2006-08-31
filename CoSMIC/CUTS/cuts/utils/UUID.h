/* -*- C++ -*- */

#ifndef _CUTS_UTILS_UUID_H_
#define _CUTS_UTILS_UUID_H_

#include "cuts/config.h"
#include "Utils_Export.h"
#include "ace/Basic_Types.h"

#pragma pack (push, 8)

class CUTS_UTILS_Export CUTS_UUID
{
public:
  /// Constuctor.
  CUTS_UUID (void);

  /// Initialize a UUID from a string.
  CUTS_UUID (const char * uuid);

  /// Copy constuctor.
  CUTS_UUID (const CUTS_UUID & uuid);

  /// Destructor.
  ~CUTS_UUID (void);

  /// Assignment operator.
  const CUTS_UUID & operator = (const CUTS_UUID & uuid);

  /// Equality operator.
  bool operator == (const CUTS_UUID & uuid) const;

  /// Inequality operator.
  bool operator != (const CUTS_UUID & uuid) const;

  /// Convert the binary version to its string representation.
  bool to_string (char * str, size_t n) const;

  /// Convert the UUID to its compressed binary representation.
  bool to_binary (char * str, size_t n) const;

private:
  void init (void);

  /// Binary representation of the UUID.
  union uuid_t
  {
    /// Binary representation using words.
    struct
    {
      /// Most-significant word 0.
      ACE_UINT32 msw0_;
      /// Most-significant word 1.
      ACE_UINT32 msw1_;
      /// Most-significant word 2.
      ACE_UINT32 msw2_;
      /// Most-significant word 3.
      ACE_UINT32 msw3_;
    } words_;

    /// Binary representation using actual fields.
    struct
    {
      /// Low-time field.
      ACE_UINT32  time_low_;
      /// Mid-time field.
      ACE_UINT16  time_mid_;
      /// High-time multiplexed w/ the version.
      ACE_UINT16  time_hi_and_version_;
      /// High clock sequence multiplexed with variant.
      ACE_UINT8   clk_seq_hi_res_;
      /// Low clock sequence.
      ACE_UINT8   clk_seq_low_;
      /// Unique node identifier.
      ACE_UINT8   node_[6];
    } fields_;

    struct
    {
      ACE_UINT8 data_[16];
    } binary_;
  } uuid_;
};

#pragma pack (pop)

#if defined (__CUTS_INLINE__)
#include "cuts/utils/UUID.inl"
#endif

#endif  // !defined _CUTS_UTILS_UUID_H_
