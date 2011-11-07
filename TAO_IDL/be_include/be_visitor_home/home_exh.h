/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    home_exh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Home node.
 *  This provides for code generation in the exec impl header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_HOME_HOME_EXH_H_
#define _BE_HOME_HOME_EXH_H_

/**
 * @class be_visitor_home_exh
 *
 * @brief be_visitor_home_exh
 *
 * This is a concrete visitor to generate the
 * exec impl header for home.
 */
class be_visitor_home_exh : public be_visitor_scope
{
public:
  be_visitor_home_exh (be_visitor_context *ctx);

  ~be_visitor_home_exh (void);

  virtual int visit_home (be_home *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_factory (be_factory *node);

  /// Helper method passed to traverse_inheritance_graph(),
  /// collects supported operations and attributes.
  static int op_attr_decl_helper (be_interface *,
                                  be_interface *,
                                  TAO_OutStream *);

private:
  int gen_exec_class (void);

  void gen_entrypoint (void);

private:
  be_home *node_;
  AST_Component *comp_;
  TAO_OutStream &os_;
  ACE_CString export_macro_;
};

#endif /* _BE_HOME_HOME_EXH_H_ */

