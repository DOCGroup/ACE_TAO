/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_cns.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO connector impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_CNS_H_
#define _BE_VISITOR_ROOT_ROOT_CNS_H_

/**
 * @class be_visitor_root_cns
 *
 * @brief be_visitor_root_cns
 *
 * This is a concrete visitor to generate the CIAO connector
 * header for root
 */
class be_visitor_root_cns : public be_visitor_root
{
public:
  be_visitor_root_cns (be_visitor_context *ctx);

  ~be_visitor_root_cns (void);

  virtual int visit_root (be_root *node);

  /// Open file and initialize stream.
  int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_CNS_H_ */
