
//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  $Id$
 *
 *  Visitors for generation of code for Arrays in client stubs. This one
 *  generates the Any operators.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ARRAY_ANY_OP_CS_H_
#define _BE_VISITOR_ARRAY_ANY_OP_CS_H_

// ************************************************************
// class be_visitor_array_cs
// ************************************************************

/**
 * @class be_visitor_array_any_op_cs
 *
 * @brief be_visitor_array_any_op_cs
 *
 * This is a concrete visitor for array that generates the Any operator
 * implementations
 */
class be_visitor_array_any_op_cs : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_array_any_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_array_any_op_cs (void);

  /// visit array
  virtual int visit_array (be_array *node);

};

#endif /* _BE_VISITOR_ARRAY_ANY_OP_CS_H_ */
