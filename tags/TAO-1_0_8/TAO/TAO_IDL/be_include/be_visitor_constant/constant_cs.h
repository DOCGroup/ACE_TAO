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
//    constant_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the constant class generating code in the client stub.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_CONSTANT_CONSTANT_CS_H_
#define _BE_VISITOR_CONSTANT_CONSTANT_CS_H_

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

#endif /* _BE_VISITOR_CONSTANT_CONSTANT_CS_H_ */
