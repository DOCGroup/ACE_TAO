/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.h
//
// = DESCRIPTION
//    Concrete visitor for the interface_fwd class
//    This one provides code generation for the CDR operators
//    (actually just their declarations) for the interface forward
//    declaration in the client inline. 
//
// = AUTHOR
//    Aniruddha Gokhale & Carlos O'Ryan
//
// ============================================================================

#ifndef BE_VISITOR_INTERFACE_FWD_CDR_OP_CI_H
#define BE_VISITOR_INTERFACE_FWD_CDR_OP_CI_H

class be_visitor_interface_fwd_cdr_op_ci : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_interface_fwd_cdr_op_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that generates the CDR
  //   operator forward declarations.
  //

public:
  be_visitor_interface_fwd_cdr_op_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_fwd_cdr_op_ci (void);
  // destructor

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface

};

#endif /* BE_VISITOR_INTERFACE_FWD_CDR_OP_CI_H */
