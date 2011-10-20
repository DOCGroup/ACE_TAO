/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_cnh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO connector impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_CNH_H_
#define _BE_VISITOR_ROOT_ROOT_CNH_H_

/**
 * @class be_visitor_root_cnh
 *
 * @brief be_visitor_root_cnh
 *
 * This is a concrete visitor to generate the CIAO connector
 * header for root.
 */
class be_visitor_root_cnh : public be_visitor_root
{
public:
  be_visitor_root_cnh (be_visitor_context *ctx);

  ~be_visitor_root_cnh (void);

  virtual int visit_root (be_root *node);

private:
  /// Open file and initialize stream.
  int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_CNH_H_ */
