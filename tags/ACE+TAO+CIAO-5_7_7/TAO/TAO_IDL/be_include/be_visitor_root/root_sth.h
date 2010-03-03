/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_sth.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  server template header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VISITOR_ROOT_ROOT_STH_H_
#define _BE_VISITOR_ROOT_ROOT_STH_H_

/**
 * @class be_visitor_root_sth
 *
 * @brief be_visitor_root_sth
 *
 * This is a concrete visitor to generate the server template
 * header for root.
 */
class be_visitor_root_sth : public be_visitor_root
{
public:
  /// Constructor.
  be_visitor_root_sth (be_visitor_context *ctx);

  /// Destructor.
  ~be_visitor_root_sth (void);

  /// Set the right context and make a visitor.
  virtual int init (void);

  // Overrides of base class methods so we can skip unnecessary nodes.

  virtual int visit_scope (be_scope *node);

  virtual int visit_module (be_module *node);

  virtual int visit_interface (be_interface *node);

  virtual int visit_component (be_component *node);
};

#endif /* _BE_VISITOR_ROOT_ROOT_STH_H_ */
