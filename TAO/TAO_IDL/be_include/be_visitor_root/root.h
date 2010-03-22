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
class be_visitor_root : public be_visitor_module
{

public:
  be_visitor_root (be_visitor_context *ctx);

  ~be_visitor_root (void);

  /// These are the only methods that differ from those of the
  /// be_visitor_module bass class.
  virtual int visit_root (be_root *node);
  virtual int visit_module (be_module *node);

  // =helper. This is used by derived clases to set the right context

  /// will be overridden by derived classes that set the appropriate context
  virtual int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_H_ */
