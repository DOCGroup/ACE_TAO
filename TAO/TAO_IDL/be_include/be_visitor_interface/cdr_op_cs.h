/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_cs.h
 *
 *  Concrete visitor for the Interface class
 *  This one provides code generation for the CDR operators for the interface
 *  in the client stub.
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
  be_visitor_interface_cdr_op_cs (be_visitor_context *ctx);
  ~be_visitor_interface_cdr_op_cs ();

  virtual int visit_interface (be_interface *node);
  virtual int visit_component (be_component *node);
  virtual int visit_connector (be_connector *node);
};

#endif /* _BE_VISITOR_INTERFACE_CDR_OP_CS_H_ */
