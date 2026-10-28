/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    home_exs.h
 *
 *  Concrete visitor for the Home node.
 *  This provides for code generation in the exec impl source
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_HOME_HOME_EXS_H_
#define _BE_HOME_HOME_EXS_H_

#include <string_view>

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
  explicit be_visitor_home_exs (be_visitor_context *ctx);

  ~be_visitor_home_exs () override;

  be_visitor_home_exs (const be_visitor_home_exs &) = delete;
  be_visitor_home_exs (be_visitor_home_exs &&) = delete;
  be_visitor_home_exs &operator= (const be_visitor_home_exs &) = delete;
  be_visitor_home_exs &operator= (be_visitor_home_exs &&) = delete;

  int visit_home (be_home *node) override;
  int visit_operation (be_operation *node) override;
  int visit_attribute (be_attribute *node) override;
  int visit_factory (be_factory *node) override;

private:
  int gen_exec_class ();

  void gen_entrypoint ();

private:
  be_home *node_ {};
  AST_Component *comp_ {};
  TAO_OutStream &os_;
  char const *const comment_start_border_ {"/**"};
  char const *const comment_end_border_ {" */"};
  char const *const your_code_here_ {"/* Your code here. */"};
  std::string_view export_macro_;
};

/// Worker class passed to traverse_inheritance_graph(),
/// collects supported operations and attributes.
class Home_Exec_Op_Attr_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  explicit Home_Exec_Op_Attr_Generator (be_visitor_scope *visitor);

  Home_Exec_Op_Attr_Generator (const Home_Exec_Op_Attr_Generator &) = delete;
  Home_Exec_Op_Attr_Generator (Home_Exec_Op_Attr_Generator &&) = delete;
  Home_Exec_Op_Attr_Generator &operator= (const Home_Exec_Op_Attr_Generator &) = delete;
  Home_Exec_Op_Attr_Generator &operator= (Home_Exec_Op_Attr_Generator &&) = delete;

  int emit (be_interface *derived_interface,
            TAO_OutStream *os,
            be_interface *base_interface) override;

private:
  be_visitor_scope *visitor_;
};

#endif /* _BE_HOME_HOME_EXS_H_ */

