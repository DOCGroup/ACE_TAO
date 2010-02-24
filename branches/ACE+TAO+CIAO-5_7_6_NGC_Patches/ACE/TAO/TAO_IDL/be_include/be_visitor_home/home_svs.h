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
//    home_svs.h
//
// = DESCRIPTION
//    Concrete visitor for the Home node.
//    This provides for code generation in the servant source
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_HOME_HOME_SVS_H_
#define _BE_HOME_HOME_SVS_H_

class be_visitor_home_svs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_home_svs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the servant header for component.
  //
  //
public:
  be_visitor_home_svs (be_visitor_context *ctx);
  
  ~be_visitor_home_svs (void);
  
  virtual int visit_home (be_home *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_argument (be_argument *node);
  
private:
  int gen_servant_class (void);
  
  int gen_ops_attrs (void);
  
  int gen_factories (void);
  int gen_factories_r (AST_Home *node);
  
  int gen_finders (void);
  int gen_finders_r (AST_Home *node);
  
  int gen_init_ops (AST_Home::INIT_LIST & list,
                    bool finder_list);
  
  void gen_entrypoint (void);
  
  /// Helper method passed to traverse_inheritance_graph(),
  /// collects supported operations and attributes.
  static int op_attr_defn_helper (be_interface *,
                                  be_interface *,
                                  TAO_OutStream *);
                     
private:
  be_home *node_;
  AST_Component *comp_;
  TAO_OutStream &os_;
  ACE_CString export_macro_;
  bool swapping_;
};

/// Worker class passed to traverse_inheritance_graph(),
/// collects supported operations and attributes.
class Home_Op_Attr_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Home_Op_Attr_Generator (be_visitor_scope * visitor);
  
  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};
                     
#endif /* _BE_HOME_HOME_SVS_H_ */

