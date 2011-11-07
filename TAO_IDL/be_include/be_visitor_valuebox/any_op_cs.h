/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the valuebox node.
 *  This one provides code generation for the Any operators
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================


#ifndef _BE_VALUEBOX_ANY_OP_CS_H_
#define _BE_VALUEBOX_ANY_OP_CS_H_

/**
 * @class be_visitor_valuebox_any_op_cs
 *
 * @brief be_visitor_valuebox_any_op_cs
 *
 * This is a concrete visitor for valuebox that generates the Any
 * operator implementations
 */
class be_visitor_valuebox_any_op_cs : public be_visitor_valuebox
{

public:
  /// constructor
  be_visitor_valuebox_any_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuebox_any_op_cs (void);

  /// visit valuebox
  virtual int visit_valuebox (be_valuebox *node);
};

#endif /* _BE_VALUEBOX_ANY_OP_CS_H_ */
