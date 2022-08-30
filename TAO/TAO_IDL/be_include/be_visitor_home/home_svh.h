/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    home_svh.h
 *
 *  Concrete visitor for the Home node.
 *  This provides for code generation in the servant header
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_HOME_HOME_SVH_H_
#define _BE_HOME_HOME_SVH_H_

/**
 * @class be_visitor_home_svh
 *
 * @brief be_visitor_home_svh
 *
 * This is a concrete visitor to generate the servant header for component.
 */
class be_visitor_home_svh : public be_visitor_scope
{
public:
  be_visitor_home_svh (be_visitor_context *ctx);

  ~be_visitor_home_svh ();

  int visit_home (be_home *node) override;
  int visit_operation (be_operation *node) override;
  int visit_attribute (be_attribute *node) override;
  int visit_factory (be_factory *node) override;
  int visit_finder (be_finder *node) override;

private:
  int gen_servant_class ();

  void gen_entrypoint ();

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

#endif /* _BE_HOME_HOME_SVH_H_ */

