
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_identifier_helper.h
 *
 *  Utilities associated with UTL_Identifier.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


#ifndef BE_IDENTIFIER_HELPER_H
#define BE_IDENTIFIER_HELPER_H

#include "utl_scoped_name.h"

#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_IDL_BE_Export.h"

class Identifier;
class be_type;
class be_visitor;

/**
 * 1) keeps escape (leading underscore character in generated identifier in IDL
 * 2) removes the '_' escape character when the identifier is
 * part of another identifier such as in provides_XXX
 * 3) removes any '_cxx_' in generated IDL
 */
struct TAO_IDL_BE_Export IdentifierHelper
{
  static std::string
  type_name (be_type *t, be_visitor *visitor);

  static Identifier *
  original_local_name (Identifier const *local_name);

  // Removes '_cxx_ from segments of a scoped name, and optionally
  // de-escape the last segment, if it's to be appended to.
  static std::string
  orig_sn (UTL_ScopedName * scoped_name, bool for_idl = true);

  // Detects case-insensitive match with IDL keyword.
  static bool
  is_idl_keyword (Identifier const *local_name);

  // Preserves the 'escape' (leading underscore) in a
  // generated identifier if necessary.
  static std::string
  try_escape (Identifier * local_name);
};

#endif /* BE_IDENTIFIER_HELPER_H */

