/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    home_svs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Home node.
 *  This provides for code generation in the servant source
 *
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

  ~be_visitor_home_svs (void);

  virtual int visit_home (be_home *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_argument (be_argument *node);
  virtual int visit_factory (be_factory *node);
  virtual int visit_finder (be_finder *node);

private:
  int gen_servant_class (void);

  void gen_entrypoint (void);

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

  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};

class be_visitor_home_attr_set : public be_visitor_decl
{
public:
  be_visitor_home_attr_set (be_visitor_context *ctx);

  ~be_visitor_home_attr_set (void);

  virtual int visit_home (be_home *node);
  virtual int visit_attribute (be_attribute *node);
};

#endif /* _BE_HOME_HOME_SVS_H_ */

