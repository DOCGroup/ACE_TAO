/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    exception_ch.h
 *
 *  $Id$
 *
 *  Visitor for Exceptions.
 *  For code generation in client header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_EXCEPTION_EXCEPTION_CH_H_
#define _BE_VISITOR_EXCEPTION_EXCEPTION_CH_H_

/**
 * @class be_visitor_exception_ch
 *
 * @brief be_visitor_exception_ch
 *
 * This is a concrete visitor to generate the client header for exception
 */
class be_visitor_exception_ch : public be_visitor_exception
{
public:
  be_visitor_exception_ch (be_visitor_context *ctx);
  ~be_visitor_exception_ch (void);

  virtual int visit_exception (be_exception *node);
};

#endif /* _BE_VISITOR_EXCEPTION_EXCEPTION_CH_H_ */
