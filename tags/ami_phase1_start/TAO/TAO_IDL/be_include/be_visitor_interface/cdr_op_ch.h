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
//    Concrete visitor for the Interface class
//    This one provides code generation for the CDR operators for the interface
//    in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_INTERFACE_CDR_OP_CH_H_
#define _BE_VISITOR_INTERFACE_CDR_OP_CH_H_

class be_visitor_interface_cdr_op_ch : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_cdr_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that generates the CDR operator
  //   declarations
  //

public:
  be_visitor_interface_cdr_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_cdr_op_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface

};

#endif /* _BE_VISITOR_INTERFACE_CDR_OP_CH_H_ */
