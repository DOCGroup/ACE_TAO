/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_svht.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO servant header.
 *
 *
 *  @author Marcel Smit
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SVTH_H_
#define _BE_VISITOR_ROOT_ROOT_SVTH_H_

/**
 * @class be_visitor_root_svth
 *
 * @brief be_visitor_root_svth
 *
 * This is a concrete visitor to generate the CIAO template servant
 * header for root
 */
class be_visitor_root_svth : public be_visitor_root
{
public:
  be_visitor_root_svth (be_visitor_context *ctx);

  ~be_visitor_root_svth (void);

  virtual int visit_root (be_root *node);

private:
  /// Open file and initialize stream.
  int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_SVTH_H_ */
