
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_xplicit_pre_proc.h
 *
 *  This visitor creates the explicit home interface
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_BE_VISITOR_XPLICIT_PRE_PROC_H
#define TAO_BE_VISITOR_XPLICIT_PRE_PROC_H

#include "be_visitor_ccm_pre_proc.h"

/**
 * @class be_visitor_xplicit_pre_proc
 *
 * @brief be_visitor_xplicit_pre_proc
 *
 * Adds home explicit implied IDL code to the AST.
 */
class be_visitor_xplicit_pre_proc : public be_visitor_ccm_pre_proc
{
public:
  be_visitor_xplicit_pre_proc (be_visitor_context *ctx);

  virtual ~be_visitor_xplicit_pre_proc ();

  int visit_home (be_home *node) override;

  int visit_operation (be_operation *node) override;
  int visit_argument (be_argument *node) override;
  int visit_factory (be_factory *node) override;
  int visit_finder (be_finder *node) override;

  int visit_interface (be_interface *node) override;
  int visit_valuebox (be_valuebox *node) override;
  int visit_valuetype (be_valuetype *node) override;
  int visit_eventtype (be_eventtype *node) override;
  int visit_structure (be_structure *node) override;
  int visit_structure_fwd (be_structure_fwd *node) override;
  int visit_exception (be_exception *node) override;
  int visit_enum (be_enum *node) override;
  int visit_field (be_field *node) override;
  int visit_attribute (be_attribute *node) override;
  int visit_union (be_union *node) override;
  int visit_union_fwd (be_union_fwd *node) override;
  int visit_union_branch (be_union_branch *node) override;
  int visit_union_label (be_union_label *node) override;
  int visit_constant (be_constant *node) override;
  int visit_enum_val (be_enum_val *node) override;
  int visit_array (be_array *node) override;
  int visit_sequence (be_sequence *node) override;
  int visit_string (be_string *node) override;
  int visit_typedef (be_typedef *node) override;
  int visit_native (be_native *node) override;
  int visit_predefined_type (be_predefined_type *node) override;

  // Member accessor
  be_interface *xplicit () const;

private:
  void check_and_store (AST_Decl *node);
  UTL_ScopedName *xplicit_iface_rel_name (AST_Decl *d);

private:
  be_interface *xplicit_;
  AST_Decl *type_holder_;
  bool ref_type_;
};

#endif // TAO_BE_VISITOR_XPLICIT_PRE_PROC_H
