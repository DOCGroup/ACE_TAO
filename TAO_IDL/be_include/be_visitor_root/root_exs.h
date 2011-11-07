/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_exs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO exec impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_EXS_H_
#define _BE_VISITOR_ROOT_ROOT_EXS_H_

/**
 * @class be_visitor_root_exs
 *
 * @brief be_visitor_root_exs
 *
 * This is a concrete visitor to generate the CIAO executor
 * header for root
 */
class be_visitor_root_exs : public be_visitor_root
{
public:
  be_visitor_root_exs (be_visitor_context *ctx);

  ~be_visitor_root_exs (void);

  virtual int visit_root (be_root *node);

private:
  /// Open file and initialize stream.
  int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_EXS_H_ */
