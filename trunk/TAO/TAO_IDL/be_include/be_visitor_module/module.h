/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    module.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Module class
 *  This one provides the generic visitor for the Module node.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_MODULE_MODULE_H_
#define _BE_VISITOR_MODULE_MODULE_H_

/**
 * @class be_visitor_module
 *
 * @brief be_visitor_module
 *
 * This is a concrete visitor for module that abstracts all common tasks.
 */
class be_visitor_module : public be_visitor_scope
{

public:
  /// constructor
  be_visitor_module (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_module (void);

  // =visit methods on all elements syntactically valid in a Module scope

  /// visit a constant
  virtual int visit_constant (be_constant *node);

  /// visit an enum
  virtual int visit_enum (be_enum *node);

  /// visit a native
  virtual int visit_native (be_native *node);

  /// visit an exception
  virtual int visit_exception (be_exception *node);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// visit a forward declared interface
  virtual int visit_interface_fwd (be_interface_fwd *node);

  /// visit the valuebox node
  virtual int visit_valuebox (be_valuebox *node);

  /// visit a valuetype
  virtual int visit_valuetype (be_valuetype *node);

  /// visit a forward declared valuetype
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  /// visit an eventtype
  virtual int visit_eventtype (be_eventtype *node);

  /// visit a forward declared eventtype
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);

  /// visit a component
  virtual int visit_component (be_component *node);

  /// visit a forward declared component
  virtual int visit_component_fwd (be_component_fwd *node);

  /// visit a home
  virtual int visit_home (be_home *node);

  /// visit a module
  virtual int visit_module (be_module *node);

  /// visit a structure
  virtual int visit_structure (be_structure *node);

  /// visit a structure
  virtual int visit_structure_fwd (be_structure_fwd *node);

  /// visit a union
  virtual int visit_union (be_union *node);

  /// visit a union
  virtual int visit_union_fwd (be_union_fwd *node);

  /// visit the typedef node
  virtual int visit_typedef (be_typedef *node);
};

#endif /* _BE_VISITOR_MODULE_MODULE_H_ */
