/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    home_exs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Home node.
 *  This provides for code generation in the exec impl source
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_HOME_HOME_EXS_H_
#define _BE_HOME_HOME_EXS_H_

/**
 * @class be_visitor_home_exs
 *
 * @brief be_visitor_home_exs
 *
 * This is a concrete visitor to generate the
 * exec impl source for home.
 */
class be_visitor_home_exs : public be_visitor_scope
{
public:
  be_visitor_home_exs (be_visitor_context *ctx);

  ~be_visitor_home_exs (void);

  virtual int visit_home (be_home *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_factory (be_factory *node);

private:
  int gen_exec_class (void);

  void gen_entrypoint (void);

private:
  be_home *node_;
  AST_Component *comp_;
  TAO_OutStream &os_;
  const char *comment_start_border_;
  const char *comment_end_border_;
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

