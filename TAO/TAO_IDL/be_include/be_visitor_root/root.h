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

  /// This is the only method that differs from that of the
  /// be_visitor_module bass class.
  virtual int visit_module (be_module *node);
};

#endif /* _BE_VISITOR_ROOT_ROOT_H_ */
