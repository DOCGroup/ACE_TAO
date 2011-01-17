/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_exh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO exec impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_EXH_H_
#define _BE_VISITOR_ROOT_ROOT_EXH_H_

/**
 * @class be_visitor_root_exh
 *
 * @brief be_visitor_root_exh
 *
 * This is a concrete visitor to generate the CIAO executor
 * header for root
 */
class be_visitor_root_exh : public be_visitor_root
{
public:
  be_visitor_root_exh (be_visitor_context *ctx);

  ~be_visitor_root_exh (void);

  virtual int visit_root (be_root *node);

private:
  /// Open file and initialize stream.
  int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_EXH_H_ */
