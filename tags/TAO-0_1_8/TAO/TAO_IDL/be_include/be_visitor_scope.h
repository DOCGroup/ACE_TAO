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
//    be_visitor_scope.h
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Scope" node
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#if !defined (TAO_BE_VISITOR_SCOPE_H)
#define TAO_BE_VISITOR_SCOPE_H

#include "be_visitor.h"
#include "be_visitor_decl.h"

class be_visitor_scope : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_scope
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_scope node. Its only purpose is to
  //   iterate over the elements of the scope
  //
public:
  be_visitor_scope (be_visitor_context *ctx);
  // constructor

  ~be_visitor_scope (void);
  // destructor

  int visit_scope (be_scope *node);
  // visit scope

  virtual int post_process (void);
  // do any processing after every element except the last one of the scope is
  // processed
};


#endif // TAO_BE_VISITOR_SCOPE_H
