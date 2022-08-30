/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    public_constructor_cs.h
 *
 *   Visitor for the Union class.
 *   This one generates code for the constructor of the union class
 *
 *  @author Johnny Willemsen
 */
//=============================================================================

#ifndef _BE_VISITOR_UNION_BRANCH_PUBLIC_CONSTRUCTOR_CS_H_
#define _BE_VISITOR_UNION_BRANCH_PUBLIC_CONSTRUCTOR_CS_H_

/**
 * @class be_visitor_union_branch_public_constructor_cs
 *
 * @brief be_visitor_union_branch_public_constructor_cs
 *
 * This is used to generate the body of the constructor
 */
class be_visitor_union_branch_public_constructor_cs : public be_visitor_decl
{
public:
  be_visitor_union_branch_public_constructor_cs (be_visitor_context *ctx);
  ~be_visitor_union_branch_public_constructor_cs ();

  int visit_union_branch (be_union_branch *node) override;

  int visit_array (be_array *node) override;
  int visit_predefined_type (be_predefined_type *node) override;
  int visit_sequence (be_sequence *node) override;
  int visit_structure (be_structure *node) override;
  int visit_structure_fwd (be_structure_fwd *node) override;
  int visit_typedef (be_typedef *node) override;
  int visit_union (be_union *node) override;
  int visit_union_fwd (be_union_fwd *node) override;
};

#endif /* _BE_VISITOR_UNION_BRANCH_PUBLIC_CONSTRUCTOR_CS_H_ */
