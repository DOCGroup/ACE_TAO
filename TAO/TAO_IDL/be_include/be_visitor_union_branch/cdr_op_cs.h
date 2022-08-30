/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    cdr_op_cs.h
 *
 *  Concrete visitor for the base "BE_Union_Branch" node
 *  This generates code for structure members in the client header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_BRANCH_CDR_OP_CS_H_
#define _BE_VISITOR_UNION_BRANCH_CDR_OP_CS_H_

/**
 * @class be_visitor_union_branch_cdr_op_cs
 *
 * @brief be_visitor_union_branch_cdr_op_cs
 *
 * This is a concrete visitor for the be_union_branch node for the client header.
 */
class be_visitor_union_branch_cdr_op_cs : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_union_branch_cdr_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_branch_cdr_op_cs ();

  /// visit the union_branch node
  int visit_union_branch (be_union_branch *node) override;

  // =visit operations on all possible data types that a union_branch can be

  int visit_array (be_array *node) override;
  int visit_enum (be_enum *node) override;
  int visit_interface (be_interface *node) override;
  int visit_interface_fwd (be_interface_fwd *node) override;
  int visit_valuebox (be_valuebox *node) override;
  int visit_valuetype (be_valuetype *node) override;
  int visit_valuetype_fwd (be_valuetype_fwd *node) override;
  int visit_predefined_type (be_predefined_type *node) override;
  int visit_sequence (be_sequence *node) override;
  int visit_string (be_string *node) override;
  int visit_structure (be_structure *node) override;
  int visit_typedef (be_typedef *node) override;
  int visit_union (be_union *node) override;

  /// extra code generation for an explicit default case
  virtual int explicit_default ();

 private:
  int emit_valuetype_common (be_type *node);
};

#endif /*  _BE_VISITOR_UNION_BRANCH_CDR_OP_CS_H_ */
