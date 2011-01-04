/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    constant_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the constant class generating code in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_CONSTANT_CONSTANT_CH_H_
#define _BE_VISITOR_CONSTANT_CONSTANT_CH_H_

/**
 * @class be_visitor_constant_ch
 *
 * @brief be_visitor_constant_ch
 *
 * This is the visitor for constant for the header file
 */
class be_visitor_constant_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_constant_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_constant_ch (void);

  /// visit constant.
  virtual int visit_constant (be_constant *node);
};

#endif /* _BE_VISITOR_CONSTANT_CONSTANT_CH_H_ */
