/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the valuetype node.
 *  This one provides code generation for the Any operators
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef _BE_VALUETYPE_ANY_OP_CS_H_
#define _BE_VALUETYPE_ANY_OP_CS_H_

/**
 * @class be_visitor_valuetype_any_op_cs
 *
 * @brief be_visitor_valuetype_any_op_cs
 *
 * This is a concrete visitor for valuetype that generates the Any
 * operator implementations
 */
class be_visitor_valuetype_any_op_cs : public be_visitor_valuetype
{

public:
  /// constructor
  be_visitor_valuetype_any_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_any_op_cs (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_VALUETYPE_ANY_OP_CS_H_ */
