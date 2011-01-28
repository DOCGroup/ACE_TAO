/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_exs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the exec impl source
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_COMPONENT_EXS_H_
#define _BE_COMPONENT_COMPONENT_EXS_H_

class be_visitor_component_exs
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_component_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the exec
  //   impl for component.
  //
  //
public:
  be_visitor_component_exs (be_visitor_context *ctx);

  ~be_visitor_component_exs (void);

  virtual int visit_component (be_component *node);
};

// =============================================

/// Worker class passed to traverse_inheritance_graph(),
/// collects supported operations and attributes. Not used
/// by this visitor but by 2 visitors spawned from this one,
/// hence the central location for the code.
class Component_Exec_Op_Attr_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Component_Exec_Op_Attr_Generator (be_visitor_scope * visitor);

  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};

#endif /* _BE_COMPONENT_COMPONENT_EXS_H_ */

