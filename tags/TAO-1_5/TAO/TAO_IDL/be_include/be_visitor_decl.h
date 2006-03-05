//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_decl.h
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Decl" node
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#ifndef TAO_BE_VISITOR_DECL_H
#define TAO_BE_VISITOR_DECL_H

#include "be_visitor.h"
#include "be_codegen.h"

class be_visitor_decl : public be_visitor
{
  //
  // = TITLE
  //    be_visitor_decl
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_decl node. Its only purpose is to
  //   maintain state information for the derived visitor
  //
public:
  be_visitor_decl (be_visitor_context *ctx);
  // constructor

  ~be_visitor_decl (void);
  // destructor

protected:
  // using a protected mechanism will allow derived visitors to access this
  // information

  be_visitor_context *ctx_;
  // any context information required by the visitor

  int gen_anonymous_base_type (be_type *bt,
                               TAO_CodeGen::CG_STATE cg_state);
  // Factors out common code used when one anonymous type is
  // contained by another.
};


#endif // TAO_BE_VISITOR_DECL_H
