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
//    Concrete visitor for the component_fwd class
//    This one provides code generation for the CDR operators
//    (actually just their declarations) for the component forward
//    declaration in the client inline.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef BE_VISITOR_COMPONENT_FWD_CDR_OP_CI_H
#define BE_VISITOR_COMPONENT_FWD_CDR_OP_CI_H

class be_visitor_component_fwd_cdr_op_ci : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_component_fwd_cdr_op_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor for component that generates the CDR
  //   operator forward declarations.
  //

public:
  be_visitor_component_fwd_cdr_op_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component_fwd_cdr_op_ci (void);
  // destructor

  virtual int visit_component_fwd (be_component_fwd *node);
  // visit interface
};

#endif /* BE_VISITOR_COMPONENT_FWD_CDR_OP_CI_H */
