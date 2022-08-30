/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    public_cs.h
 *
 *   Visitor for the Union class.
 *   This one generates code for public part of the union class for the union
 *   members in the client stubs.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_BRANCS_PUBLIC_CS_H_
#define _BE_VISITOR_UNION_BRANCS_PUBLIC_CS_H_

/**
 * @class be_visitor_union_branch_public_cs
 *
 * @brief be_visitor_union_branch_public_cs
 *
 * This visitor is used to generate implementation such as typecodes for
 * constructed types
 */
class be_visitor_union_branch_public_cs : public be_visitor_decl
{
public:
  be_visitor_union_branch_public_cs (be_visitor_context *ctx);
  ~be_visitor_union_branch_public_cs ();

  int visit_union_branch (be_union_branch *node) override;

  int visit_array (be_array *node) override;
  int visit_enum (be_enum *node) override;
  int visit_sequence (be_sequence *node) override;
  int visit_string (be_string *node) override;
  int visit_structure (be_structure *node) override;
  int visit_structure_fwd (be_structure_fwd *node) override;
  int visit_union (be_union *node) override;
  int visit_union_fwd (be_union_fwd *node) override;
};

#endif /* _BE_VISITOR_UNION_BRANCS_PUBLIC_CS_H_ */
