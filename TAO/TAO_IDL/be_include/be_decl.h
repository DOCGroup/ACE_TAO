/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_decl.h
 *
 *  $Id$
 *
 *  Extension of the AST_Decl class.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef TAO_BE_DECL_H
#define TAO_BE_DECL_H

#include "ast_decl.h"

class be_scope;
class be_visitor;
class be_type;

/**
 * @class be_decl
 *
 * @brief be_decl
 *
 * The back end extension of the AST_Decl class. Provides an abstract
 * interface.
 */
class be_decl : public virtual AST_Decl
{
public:
  be_decl (AST_Decl::NodeType type,
           UTL_ScopedName *n);

  ~be_decl (void);

  // Methods used by the interface type strategy.
  /// Both the arguments should be non-null!!!. Applies prefix and
  /// suffix to the local name and makes a flat name.
  void compute_full_name  (const char *prefix,
                           const char *suffix,
                           char *&name);

  /// Both the arguments should be non-null!!!. Applies prefix and
  /// suffix to the local name and makes a flat name.
  void compute_flat_name (const char *prefix,
                          const char *suffix,
                          char *& name);

  // End of Methods use by the interface type strategy.

  /// Return the scope created by this node (if one exists).
  virtual be_scope *scope (void);

  // Visiting
  virtual int accept (be_visitor *visitor);

  /// Cleanup function.
  virtual void destroy (void);

  /// Temporarily set this node's is_local_ flag.
  void set_local (bool val);

  // Boolean methods to test if code was already generated.
  bool cli_hdr_gen (void);
  bool cli_stub_gen (void);
  bool cli_inline_gen (void);
  bool srv_hdr_gen (void);
  bool impl_hdr_gen (void);
  bool srv_skel_gen (void);
  bool impl_skel_gen (void);
  bool srv_inline_gen (void);
  bool tie_skel_gen (void);
  bool cli_hdr_any_op_gen (void);
  bool cli_stub_any_op_gen (void);
  bool cli_hdr_cdr_op_gen (void);
  bool cli_stub_cdr_op_gen (void);
  bool cli_inline_cdr_op_gen (void);
  bool cli_inline_cdr_decl_gen (void);
  bool cli_hdr_serializer_op_gen (void);
  bool cli_stub_serializer_op_gen (void);
  bool cli_inline_serializer_op_gen (void);
  bool cli_inline_serializer_decl_gen (void);

  bool cli_traits_gen (void);
  bool cli_arg_traits_gen (void);
  bool srv_arg_traits_gen (void);
  bool srv_sarg_traits_gen (void);
  bool cli_pragma_inst_gen (void);
  bool cli_inarg_tmpl_class_gen (void);
  bool cli_inarg_pragma_inst_gen (void);
  bool cli_inoutarg_tmpl_class_gen (void);
  bool cli_inoutarg_pragma_inst_gen (void);
  bool cli_outarg_tmpl_class_gen (void);
  bool cli_outarg_pragma_inst_gen (void);
  bool cli_retarg_tmpl_class_gen (void);
  bool cli_retarg_pragma_inst_gen (void);
  bool srv_tmpl_class_gen (void);
  bool srv_pragma_inst_gen (void);
  bool srv_inarg_tmpl_class_gen (void);
  bool srv_inarg_pragma_inst_gen (void);
  bool srv_inoutarg_tmpl_class_gen (void);
  bool srv_inoutarg_pragma_inst_gen (void);
  bool srv_outarg_tmpl_class_gen (void);
  bool srv_outarg_pragma_inst_gen (void);
  bool srv_retarg_tmpl_class_gen (void);
  bool srv_retarg_pragma_inst_gen (void);
  bool ccm_pre_proc_gen (void);
  bool ex_idl_facet_gen (void);
  bool svnt_hdr_facet_gen (void);
  bool svnt_src_facet_gen (void);
  bool exec_hdr_facet_gen (void);
  bool exec_src_facet_gen (void);
  bool ami4ccm_ex_idl_gen (void);
  bool ami4ccm_rh_ex_idl_gen (void);

  // Set the flag indicating that code generation is done.
  void cli_hdr_gen (bool);
  void cli_stub_gen (bool);
  void cli_inline_gen (bool);
  void srv_hdr_gen (bool);
  void impl_hdr_gen (bool);
  void srv_skel_gen (bool);
  void impl_skel_gen (bool);
  void srv_inline_gen (bool);
  void tie_skel_gen (bool);
  void cli_hdr_any_op_gen (bool);
  void cli_stub_any_op_gen (bool);
  void cli_hdr_cdr_op_gen (bool);
  void cli_stub_cdr_op_gen (bool);
  void cli_inline_cdr_op_gen (bool);
  void cli_inline_cdr_decl_gen (bool);
  void cli_hdr_serializer_op_gen (bool);
  void cli_stub_serializer_op_gen (bool);
  void cli_inline_serializer_op_gen (bool);
  void cli_inline_serializer_decl_gen (bool);

  void cli_traits_gen (bool);
  void cli_arg_traits_gen (bool);
  void srv_arg_traits_gen (bool);
  void srv_sarg_traits_gen (bool);
  void cli_pragma_inst_gen (bool);
  void cli_inarg_tmpl_class_gen (bool);
  void cli_inarg_pragma_inst_gen (bool);
  void cli_inoutarg_tmpl_class_gen (bool);
  void cli_inoutarg_pragma_inst_gen (bool);
  void cli_outarg_tmpl_class_gen (bool);
  void cli_outarg_pragma_inst_gen (bool);
  void cli_retarg_tmpl_class_gen (bool);
  void cli_retarg_pragma_inst_gen (bool);
  void srv_tmpl_class_gen (bool);
  void srv_pragma_inst_gen (bool);
  void srv_inarg_tmpl_class_gen (bool);
  void srv_inarg_pragma_inst_gen (bool);
  void srv_inoutarg_tmpl_class_gen (bool);
  void srv_inoutarg_pragma_inst_gen (bool);
  void srv_outarg_tmpl_class_gen (bool);
  void srv_outarg_pragma_inst_gen (bool);
  void srv_retarg_tmpl_class_gen (bool);
  void srv_retarg_pragma_inst_gen (bool);
  void ccm_pre_proc_gen (bool);
  void ex_idl_facet_gen (bool);
  void svnt_hdr_facet_gen (bool);
  void svnt_src_facet_gen (bool);
  void exec_hdr_facet_gen (bool);
  void exec_src_facet_gen (bool);
  void ami4ccm_ex_idl_gen (bool);
  void ami4ccm_rh_ex_idl_gen (bool);

  // Narrowing

  DEF_NARROW_FROM_DECL (be_decl);

protected:
  // Called by be_operation (for the return type) and be_argument.
  void set_arg_seen_bit (be_type *);

private:
  // Variables that indicate if the code generation for that node is already
  // been done. This way we avoid regenerating same code.
  bool cli_hdr_gen_;
  bool cli_stub_gen_;
  bool cli_inline_gen_;
  bool srv_hdr_gen_;
  bool impl_hdr_gen_;
  bool srv_skel_gen_;
  bool impl_skel_gen_;
  bool srv_inline_gen_;
  bool tie_skel_gen_;
  bool cli_hdr_any_op_gen_;
  bool cli_stub_any_op_gen_;
  bool cli_hdr_cdr_op_gen_;
  bool cli_stub_cdr_op_gen_;
  bool cli_inline_cdr_op_gen_;
  bool cli_inline_cdr_decl_gen_;
  bool cli_hdr_serializer_op_gen_;
  bool cli_stub_serializer_op_gen_;
  bool cli_inline_serializer_op_gen_;
  bool cli_inline_serializer_decl_gen_;

  bool cli_traits_gen_;
  bool cli_arg_traits_gen_;
  bool srv_arg_traits_gen_;
  bool srv_sarg_traits_gen_;
  bool cli_pragma_inst_gen_;
  bool cli_inarg_tmpl_class_gen_;
  bool cli_inarg_pragma_inst_gen_;
  bool cli_inoutarg_tmpl_class_gen_;
  bool cli_inoutarg_pragma_inst_gen_;
  bool cli_outarg_tmpl_class_gen_;
  bool cli_outarg_pragma_inst_gen_;
  bool cli_retarg_tmpl_class_gen_;
  bool cli_retarg_pragma_inst_gen_;
  bool srv_tmpl_class_gen_;
  bool srv_pragma_inst_gen_;
  bool srv_inarg_tmpl_class_gen_;
  bool srv_inarg_pragma_inst_gen_;
  bool srv_inoutarg_tmpl_class_gen_;
  bool srv_inoutarg_pragma_inst_gen_;
  bool srv_outarg_tmpl_class_gen_;
  bool srv_outarg_pragma_inst_gen_;
  bool srv_retarg_tmpl_class_gen_;
  bool srv_retarg_pragma_inst_gen_;
  bool ccm_pre_proc_gen_;
  bool ex_idl_facet_gen_;
  bool svnt_hdr_facet_gen_;
  bool svnt_src_facet_gen_;
  bool exec_hdr_facet_gen_;
  bool exec_src_facet_gen_;
  bool ami4ccm_ex_idl_gen_;
  bool ami4ccm_rh_ex_idl_gen_;
};

#endif // if !defined
