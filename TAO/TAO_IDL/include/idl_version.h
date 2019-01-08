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
enum SpecificIdlVersion
{
  /// Invalid Version Value
  IDL_VERSION_INVALID,

  /**
   * IDL 3.x
   * tao_idl dialect of IDL defined in OMG CORBA and IDL 3.5 specs.
   * This is the version of IDL supported by the compiler as is without IDL
   * version checks.
   */
  IDL_VERSION_3,

  /**
   * IDL 4.x
   * tao_idl dialect of IDL as defined in OMG 2016-04-02 (http://www.omg.org/spec/IDL/4.0/)
   * and later revisions.
   */
  IDL_VERSION_4,

  /**
   * SHOULD ALWAYS BE LAST, DO NOT PUT ANY VERSIONS PAST THIS!
   *
   * Attempting to pass this to IdlVersion will result in an invalid value.
   */
  IDL_VERSION_COUNT
};

/**
 * Default version of IDL to use if BE does not specify a default version.
 * This is so that BE can control the version without overriding idl_version_
 * directly and making --default-idl-version incorrect.
 */
const SpecificIdlVersion DEFAULT_DEFAULT_IDL_VERSION = IDL_VERSION_3;

/**
 * Class Operations involving SpecificIdlVersion Values
 */
class TAO_IDL_FE_Export IdlVersion
{
public:
  /**
   * New IdlVersion set to DEFAULT_DEFAULT_IDL_VERSION.
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

#endif
