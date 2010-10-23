/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    executor_exh.h
 *
 *  $Id$
 *
 *  This provides code generation in the exec impl header,
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_EXECUTOR_EXH_H_
#define _BE_COMPONENT_EXECUTOR_EXH_H_

class be_visitor_executor_exh
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_executor_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor implementation class declaration.
  //
  //
public:
  be_visitor_executor_exh (be_visitor_context *ctx);

  ~be_visitor_executor_exh (void);

  virtual int visit_attribute (be_attribute *node);
  virtual int visit_component (be_component *node);
  virtual int visit_provides (be_provides *node);
  virtual int visit_consumes (be_consumes *node);
private:
  const char *comment_border_;
};

// =============================================

/// Worker class passed to traverse_inheritance_graph(),
/// generates code to declare attribute members in
/// the executor impl class.
class Exec_Attr_Decl_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Exec_Attr_Decl_Generator (
    be_visitor_scope * visitor);

  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};

#endif /* _BE_COMPONENT_EXECUTOR_EXH_H_ */

