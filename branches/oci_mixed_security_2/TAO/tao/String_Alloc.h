// -*- C++ -*-
//=============================================================================
/**
 *  @file    String_Alloc.h
 *
 *  $Id$
 *
 *  Header file for the CORBA string types.
 *
 *  @author DOC Group at Wash U, UCI, and Vanderbilt U.
 */
//=============================================================================

#ifndef TAO_STRING_ALLOC_H
#define TAO_STRING_ALLOC_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"
#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  /**
   * @name CORBA String Memory Management
   *
   * CORBA string memory management functions.
   */
  //@{
  TAO_Export char * string_alloc (ULong len);
  TAO_Export char * string_dup (const char *);
  TAO_Export void string_free (char *);
  //@}

  /**
   * @name CORBA Wide String Memory Management
   *
   * CORBA wide string memory management functions.
   */
  //@{
  TAO_Export WChar * wstring_alloc (ULong len);
  TAO_Export WChar * wstring_dup (const WChar * const);
  TAO_Export void wstring_free (WChar * const);
  //@}
}  // End CORBA namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_STRING_ALLOC_H */
