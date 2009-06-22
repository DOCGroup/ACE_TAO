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
//    cdr_op_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the ComponentFwd class
//    This one provides code generation for the CDR operators for the forward
//    declared component in the client header, if the node is not later defined
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VISITOR_COMPONENT_FWD_CDR_OP_CH_H_
#define _BE_VISITOR_COMPONENT_FWD_CDR_OP_CH_H_

class be_visitor_component_fwd_cdr_op_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_component_fwd_cdr_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for forward declared component that generates
  //   the CDR operator declarations, if the node is not later defined.
  //

public:
  be_visitor_component_fwd_cdr_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component_fwd_cdr_op_ch (void);
  // destructor

  virtual int visit_component_fwd (be_component_fwd *node);
  // visit forward declared component.
};

#endif /* _BE_VISITOR_COMPONENT_FWD_CDR_OP_CH_H_ */
