
/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  Concrete visitor for the Typedef class
 *  This provides the code generation for the Any operators of the Typedef
 *  class in the client stubs.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_TYPEDEF_ANY_OP_CS_H_
#define _BE_VISITOR_TYPEDEF_ANY_OP_CS_H_

/**
 * @class be_visitor_typedef_any_op_cs
 *
 * @brief be_visitor_typedef_any_op_cs
 *
 * This is a concrete visitor for typedef that generates the Any operator
 * declarations and implementations
 */
class be_visitor_typedef_any_op_cs : public be_visitor_typedef
{

public:
  /// constructor
  be_visitor_typedef_any_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_typedef_any_op_cs ();

  /// visit typedef
  int visit_typedef (be_typedef *node) override;

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  /// visit a array
  int visit_array (be_array *node) override;

  /// visit an enum
  int visit_enum (be_enum *node) override;

  /// visit a sequence
  int visit_sequence (be_sequence *node) override;

  /// visit a structure
  int visit_structure (be_structure *node) override;

  /// visit a union
  int visit_union (be_union *node) override;

};

#endif /* _BE_VISITOR_TYPEDEF_ANY_OP_CS_H_*/
