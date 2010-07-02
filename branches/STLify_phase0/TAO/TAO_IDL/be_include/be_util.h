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
 *  @author Gary Maxey
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_BE_UTIL_H
#define TAO_BE_UTIL_H

class TAO_OutStream;
class be_module;
class AST_Decl;

// (JP) Someday these should be consolidated, along with several
// similar methods scattered throughout the backend.
class be_util
{
public:
  static void
  gen_nested_namespace_begin (TAO_OutStream *os, be_module *node);

  static void
  gen_nested_namespace_end (TAO_OutStream *os, be_module *node);

  static void
  gen_nesting_open (TAO_OutStream &os, AST_Decl *node);
  
  static void
  gen_nesting_close (TAO_OutStream &os, AST_Decl *node);
};

#endif // if !defined

