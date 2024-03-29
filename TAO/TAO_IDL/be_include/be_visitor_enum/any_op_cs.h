/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  Concrete visitor for Enums generating code for the Any operators
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ENUM_ANY_OP_CS_H_
#define _BE_VISITOR_ENUM_ANY_OP_CS_H_

/**
 * @class be_visitor_enum_any_op_cs
 *
 * @brief be_visitor_enum_any_op_cs
 *
 * This is a concrete visitor for enum that generates the Any operator
 * implementations
 */
class be_visitor_enum_any_op_cs : public be_visitor_scope
{
public:
  /// constructor
  be_visitor_enum_any_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_enum_any_op_cs ();

  /// visit enum
  virtual int visit_enum (be_enum *node);
};

#endif /* _BE_VISITOR_ENUM_ANY_OP_CS_H_ */
