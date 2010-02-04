/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface class
 *  This one provides code generation for the CDR operators for the interface
 *  in the client stub.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_INTERFACE_CDR_OP_CS_H_
#define _BE_VISITOR_INTERFACE_CDR_OP_CS_H_

/**
 * @class be_visitor_interface_cdr_op_cs
 *
 * @brief be_visitor_interface_cdr_op_cs
 *
 * This is a concrete visitor for interface that generates the CDR operator
 * implementations
 */
class be_visitor_interface_cdr_op_cs : public be_visitor_interface
{

public:
  /// constructor
  be_visitor_interface_cdr_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_interface_cdr_op_cs (void);

  /// visit interface
  virtual int visit_interface (be_interface *node);
};

#endif /* _BE_VISITOR_INTERFACE_CDR_OP_CS_H_ */
