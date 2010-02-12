/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides the generic visitor for the Root node.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ROOT_ROOT_H_
#define _BE_VISITOR_ROOT_ROOT_H_

/**
 * @class be_visitor_root
 *
 * @brief be_visitor_root
 *
 * This is a concrete visitor for root that abstracts all common tasks
 */
class be_visitor_root : public be_visitor_scope
{

public:
  be_visitor_root (be_visitor_context *ctx);

  ~be_visitor_root (void);

  virtual int visit_root (be_root *node);
  virtual int visit_constant (be_constant *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_exception (be_exception *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_eventtype (be_eventtype *node);
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);
  virtual int visit_component (be_component *node);
  virtual int visit_component_fwd (be_component_fwd *node);
  virtual int visit_home (be_home *node);
  virtual int visit_connector (be_connector *node);
  virtual int visit_module (be_module *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_structure_fwd (be_structure_fwd *node);
  virtual int visit_union (be_union *node);
  virtual int visit_union_fwd (be_union_fwd *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_native (be_native *node);

  // =helper. This is used by derived clases to set the right context

  /// will be overridden by derived classes that set the appropriate context
  virtual int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_H_ */
