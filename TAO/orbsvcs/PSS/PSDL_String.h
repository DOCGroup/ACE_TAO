/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PSDL_String.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef TAO_PSDL_STRING_H
#define TAO_PSDL_STRING_H
#include "ace/pre.h"

#include "psdl_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PSDL_Datastore.h"

/**
 * @class TAO_PSDL_String
 *
 * @brief Wrapper for the ACE_CString to facilitate saving the string
 * persistently.
 *
 *
 **/

class ACE_Allocator;

class TAO_PSDL_Export TAO_PSDL_String
{
public:

  /// Constructor.
  TAO_PSDL_String (void);

  /// Constructor.
  TAO_PSDL_String (ACE_Allocator *persistent_allocator);

  /// Copy constructor.
  TAO_PSDL_String (const TAO_PSDL_String & rhs);

  /// Destructor.
  ~TAO_PSDL_String (void);

  /// Assignment operator.
  void operator= (const TAO_PSDL_String & rhs);

  // Conversion operator (assignment)
  void operator= (const ACE_CString & rhs);

  /// Equality comparison operator.
  int operator== (const TAO_PSDL_String &rhs) const;

  /// Inequality comparison operator.
  int operator!= (const TAO_PSDL_String &rhs) const;

  /// Returns a hash value for this string.
  u_long hash (void) const;

  // Conversion operators (cast)
  // (caller owns storage of return values)
  operator ACE_CString *() const;

  operator ACE_CString *();

  // Persistent allocator.
  ACE_Allocator * allocator_;

  // Persistent buffer.
  const char * buffer_;

  // Length of the persistent buffer.
  CORBA::ULong length_;
};


#include "ace/post.h"
#endif /* TAO_PSDL_STRING_H */
