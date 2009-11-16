//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    home_exs.h
//
// = DESCRIPTION
//    Concrete visitor for the Home node.
//    This provides for code generation in the exec impl source
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_HOME_HOME_EXS_H_
#define _BE_HOME_HOME_EXS_H_

class be_visitor_home_exs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_home_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the
  //   exec impl source for home.
  //
  //
public:
  be_visitor_home_exs (be_visitor_context *ctx);
  
  ~be_visitor_home_exs (void);
  
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
  
private:
  be_home *node_;
  AST_Component *comp_;
  TAO_OutStream &os_;
  const char *comment_border_;
  const char *your_code_here_;
  ACE_CString export_macro_;
};

/// Worker class passed to traverse_inheritance_graph(),
/// collects supported operations and attributes.
class Home_Exec_Op_Attr_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Home_Exec_Op_Attr_Generator (be_visitor_scope * visitor);
  
  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};
                     
#endif /* _BE_HOME_HOME_EXS_H_ */

