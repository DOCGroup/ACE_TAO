/**
 * Header File for Tracking IDL Versions
 *
 * Source File Counterpart is util/idl_version.cpp.
 */

#ifndef IDL_VERSION_HEADER
#define IDL_VERSION_HEADER

#include "TAO_IDL_FE_Export.h"

/**
 * List of Specific Idl Versions
 *
 * Order is used to determine how versions compare to each other.
 *
 * NOTE: If updated, idlVersionNames in util/idl_version.cpp must be updated
 * as well!
 */
enum TAO_IDL_FE_Export SpecificIdlVersion
{
  /// Invalid Version Value
  IDL_VERSION_INVALID,

  /**
   * TODO: Determine what this should be named
   */
  IDL_VERSION_3,

  /**
   * IDL 4.0 as defined in OMG 2016-04-02 (http://www.omg.org/spec/IDL/4.0/)
   */
  IDL_VERSION_4_0,

  /// Attempting to pass this to IdlVersion will result in an invalid value.
  IDL_VERSION_COUNT
};

/*
 * Default version of IDL to use if no other options are given.
 */
const SpecificIdlVersion DEFAULT_IDL_VERSION = IDL_VERSION_4_0;

/**
 * Class Operations involving SpecificIdlVersion Values
 */
class TAO_IDL_FE_Export IdlVersion
{
public:
  /**
   * New IdlVersion set to DEFAULT_IDL_VERSION.
   */
  IdlVersion ();

  /**
   * New IdlVersion set from a SpecificIdlVersion if version is valid,
   * otherwise IDL_VERSION_INVALID.
   */
  IdlVersion (SpecificIdlVersion version);

  /**
   * Get version as a SpecificIdlVersion.
   */
  SpecificIdlVersion version () const;
  /**
   * Set version from a SpecificIdlVersion if version is valid, otherwise
   * IDL_VERSION_INVALID.
   */
  void version (SpecificIdlVersion version);

  /**
   * Is Version Valid?
   */
  bool is_valid () const;

  /**
   * Get version as string.
   */
  const char * to_string () const;
  /**
   * Set version from string, sets versions as invalid if it does not match
   * any strings in idlVersionNames.
   */
  void from_string (const char * version);

  /**
   * Compare a IdlVersion to a SpecificIdlVersion or another IdlVersion
   */
  ///{
  operator SpecificIdlVersion () const;
  bool operator== (SpecificIdlVersion other) const;
  bool operator!= (SpecificIdlVersion other) const;
  bool operator> (SpecificIdlVersion other) const;
  bool operator>= (SpecificIdlVersion other) const;
  bool operator< (SpecificIdlVersion other) const;
  bool operator<= (SpecificIdlVersion other) const;
  ///}

private:
  SpecificIdlVersion version_;
};

/**
 * Compare a SpecificIdlVersion to a IdlVersion
 */
///{
/*
bool operator== (SpecificIdlVersion left, const IdlVersion &right);
bool operator!= (SpecificIdlVersion left, const IdlVersion &right);
bool operator> (SpecificIdlVersion left, const IdlVersion &right);
bool operator>= (SpecificIdlVersion left, const IdlVersion &right);
bool operator< (SpecificIdlVersion left, const IdlVersion &right);
bool operator<= (SpecificIdlVersion left, const IdlVersion &right);
*/
///}

#endif
