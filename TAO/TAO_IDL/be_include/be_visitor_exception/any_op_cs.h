/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  Visitor for Exceptions.
 *  For code generation of Any operator in client stubs
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
  ~be_visitor_exception_any_op_cs () override;

  int visit_exception (be_exception *node) override;
  int visit_field (be_field *node) override;
  int visit_enum (be_enum *node) override;
  int visit_structure (be_structure *node) override;
  int visit_union (be_union *node) override;
};

#endif /* _BE_VISITOR_EXCEPTION_ANY_OP_CS_H_ */
