// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL3_TO_IDL2_BE_DLL
//
// = FILENAME
//    identifier_helper.h
//
// = DESCRIPTION
//    Utilities associated with UTL_Identifier.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef IDENTIFIER_HELPER_H
#define IDENTIFIER_HELPER_H

#include "utl_identifier.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "TAO_IDL3_TO_IDL2_BE_Export.h"
#include "ace/SString.h"

struct TAO_IDL3_TO_IDL2_BE_Export IdentifierHelper
{
  /*
    1) keeps escape (leading underscore character in generated
       identifier in IDL
    2) removes the '_' escape character when the identifier is
       part of another identifier such as in provides_XXX
    3) removes any '_cxx_' in generated IDL (equivalent to
       AST_Decl::original_local_name())
    TODO: port identifiers
  */
  static Identifier * 
  original_local_name (Identifier * local_name);

  // Detects case-insensitive match with IDL keyword.
  static bool
  is_idl_keyword (Identifier * local_name);
  
  // Preserves the 'escape' (leading underscore) in a
  // generated identifier if necessary.
  static ACE_CString 
  try_escape (Identifier * local_name);
};

#endif /* IDENTIFIER_HELPER_H */

