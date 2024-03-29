/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_util.h
 *
 *  Static helper methods used by multiple visitors.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_BE_UTIL_H
#define TAO_BE_UTIL_H

#include "TAO_IFR_BE_Export.h"

class AST_Generator;

class be_util
{
public:
  /// Special BE arg call factored out of DRV_args.
  static TAO_IFR_BE_Export void
  prep_be_arg (char *s);

  /// Checks made after parsing args.
  static TAO_IFR_BE_Export void
  arg_post_proc ();

  /// Display usage of BE-specific options.
  static TAO_IFR_BE_Export void
  usage ();

  /// Create an AST node generator.
  static TAO_IFR_BE_Export AST_Generator *
  generator_init ();
};

#endif // if !defined

