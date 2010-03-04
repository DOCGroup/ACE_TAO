/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    exception.h
 *
 *  $Id$
 *
 *  Visitor for Exceptions.
 *  This is the generic (common) visitor for all exceptions.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_EXCEPTION_EXCEPTION_H_
#define _BE_VISITOR_EXCEPTION_EXCEPTION_H_

/**
 * @class be_visitor_exception
 *
 * @brief be_visitor_exception
 *
 * This is the base visitor for exception
 */
class be_visitor_exception : public be_visitor_scope
{
public:
  /// ctor
  be_visitor_exception (be_visitor_context *ctx);

  /// dtor
  ~be_visitor_exception (void);

  /// visit exception. We provide code for this method in the derived class
  virtual int visit_exception (be_exception *node);

  // =visit operations on syntactically valid elements in our scope

  /// visit field
  virtual int visit_field (be_field *node);
};

#endif /* _BE_VISITOR_EXCEPTION_EXCEPTION_H_ */
