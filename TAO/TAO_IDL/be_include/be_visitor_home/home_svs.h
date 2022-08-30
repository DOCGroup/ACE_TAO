/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    home_svs.h
 *
 *  Concrete visitor for the Home node.
 *  This provides for code generation in the servant source
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_HOME_HOME_SVS_H_
#define _BE_HOME_HOME_SVS_H_

/**
 * @class be_visitor_home_svs
 *
 * @brief be_visitor_home_svs
 *
 * This is a concrete visitor to generate the servant
 * source for a comopnent home.
 */
class be_visitor_home_svs : public be_visitor_scope
{
public:
  be_visitor_home_svs (be_visitor_context *ctx);

  ~be_visitor_home_svs ();

  int visit_home (be_home *node) override;
  int visit_operation (be_operation *node) override;
  int visit_attribute (be_attribute *node) override;
  int visit_argument (be_argument *node) override;
  int visit_factory (be_factory *node) override;
  int visit_finder (be_finder *node) override;

private:
  int gen_servant_class ();

  void gen_entrypoint ();

private:
  be_home *node_;
  AST_Component *comp_;
  TAO_OutStream &os_;
  ACE_CString export_macro_;
  bool for_finder_;
};

/// Worker class passed to traverse_inheritance_graph(),
/// collects supported operations and attributes.
class Home_Op_Attr_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Home_Op_Attr_Generator (be_visitor_scope * visitor);

  int emit (be_interface * derived_interface,
            TAO_OutStream * os,
            be_interface * base_interface) override;

private:
  be_visitor_scope * visitor_;
};

class be_visitor_home_attr_set : public be_visitor_decl
{
public:
  be_visitor_home_attr_set (be_visitor_context *ctx);

  ~be_visitor_home_attr_set ();

  int visit_home (be_home *node) override;
  int visit_attribute (be_attribute *node) override;
};

#endif /* _BE_HOME_HOME_SVS_H_ */

