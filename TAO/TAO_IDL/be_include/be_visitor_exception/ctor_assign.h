/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ctor_assign.h
 *
 *  Visitor for Exceptions.
 *  For code generation for the special constructor and assignment operator
 *  for exceptions.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_EXCEPTION_CTOR_ASSIGN_H_
#define _BE_VISITOR_EXCEPTION_CTOR_ASSIGN_H_

/**
 * @class be_visitor_exception_ctor_assign
 *
 * generation of the assignment statements in
 */
class be_visitor_exception_ctor_assign : public be_visitor_scope
{
public:
  be_visitor_exception_ctor_assign (be_visitor_context *ctx);
  ~be_visitor_exception_ctor_assign () override;

  int visit_exception (be_exception *node) override;
  int visit_field (be_field *node) override;
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
  int visit_structure_fwd (be_structure_fwd *node) override;
  int visit_union (be_union *node) override;
  int visit_union_fwd (be_union_fwd *node) override;
  int visit_typedef (be_typedef *node) override;

private:
  int emit_valuetype_common (be_type *node);
};

#endif /* _BE_VISITOR_EXCEPTION_CTOR_ASSIGN_H_ */
