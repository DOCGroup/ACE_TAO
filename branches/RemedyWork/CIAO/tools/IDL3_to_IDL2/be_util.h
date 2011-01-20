/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_util.h
 *
 *  $Id$
 *
 *  Static helper methods used by multiple visitors.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_BE_UTIL_H
#define TAO_BE_UTIL_H

#include "TAO_IDL3_TO_IDL2_BE_Export.h"

class AST_Generator;

class be_util
{
public:
  /// Special BE arg call factored out of DRV_args.
  static TAO_IDL3_TO_IDL2_BE_Export void
  prep_be_arg (char *s);

  /// Checks made after parsing args.
  static TAO_IDL3_TO_IDL2_BE_Export void
  arg_post_proc (void);

  /// Display usage of BE-specific options.
  static TAO_IDL3_TO_IDL2_BE_Export void
  usage (void);

  /// Create an AST node generator.
  static TAO_IDL3_TO_IDL2_BE_Export AST_Generator *
  generator_init (void);
};

#endif // if !defined

