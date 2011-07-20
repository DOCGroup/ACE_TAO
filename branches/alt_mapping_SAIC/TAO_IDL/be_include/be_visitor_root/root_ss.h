/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_ss.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  server skeletons.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SS_H_
#define _BE_VISITOR_ROOT_ROOT_SS_H_

/**
 * @class be_visitor_root_ss
 *
 * @brief be_visitor_root_ss
 *
 * This is a concrete visitor to generate the server skeletons for root
 */
class be_visitor_root_ss : public be_visitor_root
{
public:
  be_visitor_root_ss (be_visitor_context *ctx);

  ~be_visitor_root_ss (void);

  virtual int visit_root (be_root *node);

private:
  /// Open file and initialize stream.
  int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_SS_H_ */
