/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  Concrete visitor for the base "BE_Union_Branch" node
 *  This generates code for structure members in the client header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_BRANCH_CDR_OP_CH_H_
#define _BE_VISITOR_UNION_BRANCH_CDR_OP_CH_H_

/**
 * @class be_visitor_union_branch_cdr_op_ch
 *
 * @brief be_visitor_cdr_op_ch
 *
 * This is a concrete visitor for the be_union_branch node for the client header.
 */
class be_visitor_union_branch_cdr_op_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_union_branch_cdr_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_branch_cdr_op_ch ();

  /// visit the union_branch node
  int visit_union_branch (be_union_branch *node) override;

  // =visit operations on all possible data types that a union_branch can be

  /// visit array type
  int visit_array (be_array *node) override;

  /// visit enum type
  int visit_enum (be_enum *node) override;

  /// visit sequence type
  int visit_sequence (be_sequence *node) override;

  /// visit structure type
  int visit_structure (be_structure *node) override;

  /// visit typedefed type
  int visit_typedef (be_typedef *node) override;

  /// visit union type
  int visit_union (be_union *node) override;
};

#endif /*  _BE_VISITOR_UNION_BRANCH_CDR_OP_CH_H_ */
