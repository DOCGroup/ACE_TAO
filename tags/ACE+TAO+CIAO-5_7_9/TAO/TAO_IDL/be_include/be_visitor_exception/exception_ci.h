/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    exception_ci.h
 *
 *  $Id$
 *
 *  Visitor for Exceptions.
 *  For code generation in client inline.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_EXCEPTION_EXCEPTION_CI_H_
#define _BE_VISITOR_EXCEPTION_EXCEPTION_CI_H_

/**
 * @class be_visitor_exception_ci
 *
 * @brief be_visitor_exception_ci
 *
 * This is a concrete visitor to generate the client inline for exception
 */
class be_visitor_exception_ci : public be_visitor_exception
{
public:
  /// ctor
  be_visitor_exception_ci (be_visitor_context *ctx);

  /// dtor
  ~be_visitor_exception_ci (void);

  /// visit exception.
  virtual int visit_exception (be_exception *node);

};

#endif /* _BE_VISITOR_EXCEPTION_EXCEPTION_CI_H_ */
