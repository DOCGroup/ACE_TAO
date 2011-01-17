/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_svs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO servant source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SVS_H_
#define _BE_VISITOR_ROOT_ROOT_SVS_H_

/**
 * @class be_visitor_root_svs
 *
 * @brief be_visitor_root_svs
 *
 * This is a concrete visitor to generate the CIAO servant
 * source for root
 */
class be_visitor_root_svs : public be_visitor_root
{
public:
  be_visitor_root_svs (be_visitor_context *ctx);

  ~be_visitor_root_svs (void);

  virtual int visit_root (be_root *node);

private:
  /// Set the right context and make a visitor.
  int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_SVS_H_ */
