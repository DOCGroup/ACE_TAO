
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor_xplicit_pre_proc.h
 *
 *  $Id$
 *
 *  This visitor creates the explicit home interface
 *
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

  virtual ~be_visitor_xplicit_pre_proc (void);

  virtual int visit_home (be_home *node);

  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);
  virtual int visit_factory (be_factory *node);
  virtual int visit_finder (be_finder *node);

  virtual int visit_interface (be_interface *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_structure_fwd (be_structure_fwd *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_field (be_field *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_union (be_union *node);
  virtual int visit_union_fwd (be_union_fwd *node);
  virtual int visit_union_branch (be_union_branch *node);
  virtual int visit_union_label (be_union_label *node);
  virtual int visit_constant (be_constant *node);
  virtual int visit_enum_val (be_enum_val *node);
  virtual int visit_array (be_array *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_string (be_string *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_native (be_native *node);
  virtual int visit_predefined_type (be_predefined_type *node);

  // Member accessor
  be_interface *xplicit (void) const;

private:
  void check_and_store (AST_Decl *node);
  UTL_ScopedName *xplicit_iface_rel_name (AST_Decl *d);

private:
  be_interface *xplicit_;
  AST_Decl *type_holder_;
  bool ref_type_;
};

#endif // TAO_BE_VISITOR_XPLICIT_PRE_PROC_H
