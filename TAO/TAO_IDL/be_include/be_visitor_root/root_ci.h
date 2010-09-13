/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_ci.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  client inline.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_CI_H_
#define _BE_VISITOR_ROOT_ROOT_CI_H_

/**
 * @class be_visitor_root_ci
 *
 * @brief be_visitor_root_ci
 *
 * This is a concrete visitor to generate the
 * client inline for root
 */
class be_visitor_root_ci : public be_visitor_root
{
public:
  be_visitor_root_ci (be_visitor_context *ctx);

  ~be_visitor_root_ci (void);

  virtual int visit_root (be_root *node);

private:
  /// Open file and initialize stream.
  int init (void);

  /// Generate OBV definitions in the client inline
  /// file, done after the main scope traversal.
  int gen_obv_defns (be_root *node);
};

#endif /* _BE_VISITOR_ROOT_ROOT_CI_H_ */
