/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_svts.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO servant source.
 *
 *
 *  @author Marcel Smit
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SVTS_H_
#define _BE_VISITOR_ROOT_ROOT_SVTS_H_

/**
 * @class be_visitor_root_svts
 *
 * @brief be_visitor_root_svts
 *
 * This is a concrete visitor to generate the CIAO template servant
 * source for root
 */
class be_visitor_root_svts : public be_visitor_root
{
public:
  be_visitor_root_svts (be_visitor_context *ctx);

  ~be_visitor_root_svts (void);

  virtual int visit_root (be_root *node);

private:
  /// Set the right context and make a visitor.
  int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_SVTS_H_ */
