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
//    in the client stub.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_INTERFACE_CDR_OP_CS_H_
#define _BE_VISITOR_INTERFACE_CDR_OP_CS_H_

class be_visitor_interface_cdr_op_cs : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_cdr_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for interface that generates the CDR operator
  //   implementations
  //

public:
  be_visitor_interface_cdr_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_cdr_op_cs (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface

};

#endif /* _BE_VISITOR_INTERFACE_CDR_OP_CS_H_ */
