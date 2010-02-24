//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    home_exh.h
//
// = DESCRIPTION
//    Concrete visitor for the Home node.
//    This provides for code generation in the exec impl header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_HOME_HOME_EXH_H_
#define _BE_HOME_HOME_EXH_H_

class be_visitor_home_exh : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_home_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the
  //   exec impl header for home.
  //
  //
public:
  be_visitor_home_exh (be_visitor_context *ctx);
  
  ~be_visitor_home_exh (void);
  
  virtual int visit_home (be_home *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  
private:
  int gen_exec_class (void);
  
  int gen_ops_attrs (void);
  
  int gen_factories (void);
  int gen_factories_r (AST_Home *node);
  
  int gen_finders (void);
  int gen_finders_r (AST_Home *node);
  
  int gen_init_ops_i (AST_Home::INIT_LIST & list);
  
  void gen_entrypoint (void);
  
  /// Helper method passed to traverse_inheritance_graph(),
  /// collects supported operations and attributes.
  static int op_attr_decl_helper (be_interface *,
                                  be_interface *,
                                  TAO_OutStream *);
                     
private:
  be_home *node_;
  AST_Component *comp_;
  TAO_OutStream &os_;
  ACE_CString export_macro_;
};

#endif /* _BE_HOME_HOME_EXH_H_ */

