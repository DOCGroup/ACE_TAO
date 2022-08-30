/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    field_ci.h
 *
 *  Concrete visitor for the base "BE_Field" node
 *  This generates code for structure members in the client inline.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_FIELD_CI_H_
#define _BE_VISITOR_FIELD_CI_H_

/**
 * @class be_visitor_field_ci
 *
 * @brief be_visitor_field_ci
 *
 * This is a concrete visitor for the be_field node for the client inline
 * file
 */
class be_visitor_field_ci : public be_visitor_decl
{
public:
  be_visitor_field_ci (be_visitor_context *ctx);
  ~be_visitor_field_ci () override;

  int visit_field (be_field *node) override;

  int visit_array (be_array *node) override;
  int visit_sequence (be_sequence *node) override;
  int visit_structure (be_structure *node) override;
  int visit_structure_fwd (be_structure_fwd *node) override;
  int visit_typedef (be_typedef *node) override;
  int visit_union (be_union *node) override;
  int visit_union_fwd (be_union_fwd *node) override;
};

#endif /*  _BE_VISITOR_FIELD_CI_H_ */
