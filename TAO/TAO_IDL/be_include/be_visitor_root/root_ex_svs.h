/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_ex_svs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO executor servant.
 *
 *
 *  @author Marcel Smit
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_EX_SVNT_H_
#define _BE_VISITOR_ROOT_ROOT_EX_SVNT_H_

/**
 * @class be_visitor_root_ex_svs
 *
 * @brief be_visitor_root_ex_svs
 *
 * This is a concrete visitor to generate the CIAO executor
 * IDL for root
 */
class be_visitor_root_ex_svs : public be_visitor_root
{
public:
  be_visitor_root_ex_svs (be_visitor_context *ctx);

  ~be_visitor_root_ex_svs (void);

  virtual int visit_root (be_root *node);

private:
  /// Open file and initialize stream.
  int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_EX_IDL_H_ */
