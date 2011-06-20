/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  $Id$
 *
 *  Visitor for Exceptions.
 *  For code generation of Any operator in client stubs
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_EXCEPTION_ANY_OP_CS_H_
#define _BE_VISITOR_EXCEPTION_ANY_OP_CS_H_

/**
 * @class be_visitor_exception_any_op_cs
 *
 * @brief be_visitor_exception_any_op_cs
 *
 * This is a concrete visitor for exception that generates the Any operator
 * implementations
 */
class be_visitor_exception_any_op_cs : public be_visitor_scope
{

public:
  be_visitor_exception_any_op_cs (be_visitor_context *ctx);
  ~be_visitor_exception_any_op_cs (void);

  virtual int visit_exception (be_exception *node);
  virtual int visit_field (be_field *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_union (be_union *node);
};

#endif /* _BE_VISITOR_EXCEPTION_ANY_OP_CS_H_ */
