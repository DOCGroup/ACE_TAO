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

#ifndef TAO_BE_VISITOR_SCOPE_H
#define TAO_BE_VISITOR_SCOPE_H

#include "idl_bool.h"
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

  virtual int post_process (be_decl *);
  // do any processing after every element except the last one of the scope is
  // processed

  virtual int pre_process (be_decl *);
  // do any processing prior to processing the element in the scope

  virtual int elem_number (void);
  // return the element number that is being processed

  virtual int next_elem (be_decl *elem, be_decl *&);
  // get the successor to elem

  virtual idl_bool last_node (be_decl *bd);
  // Is bd the last node in the scope

  idl_bool last_inout_or_out_node (be_decl *bd);
  // Is bd the last inout or out in the scope

protected:
  int elem_number_;
};


#endif // TAO_BE_VISITOR_SCOPE_H
