/* -*- C++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_constant.h
//
// = DESCRIPTION
//    Concrete visitor for the constant class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_CONSTANT_H)
#define TAO_BE_VISITOR_CONSTANT_H

#include "be_visitor.h"
#include "be_visitor_scope.h"

class be_visitor_constant_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_constant_ch
  //
  // = DESCRIPTION
  //   This is the visitor for constant for the header file
  //
  //
public:
  be_visitor_constant_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_constant_ch (void);
  // destructor

  virtual int visit_constant (be_constant *node);
  // visit constant.

};

class be_visitor_constant_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_constant_cs
  //
  // = DESCRIPTION
  //   This is the visitor for constant for the impl file
  //
  //
public:
  be_visitor_constant_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_constant_cs (void);
  // destructor

  virtual int visit_constant (be_constant *node);
  // visit constant

  // = special methods

  virtual int gen_nested_namespace_begin (be_module *node);
  // generate the nested namespace begins

  virtual int gen_nested_namespace_end (be_module *node);
  // generate the nested namespace ends
};

#endif // TAO_BE_VISITOR_CONSTANT_H
