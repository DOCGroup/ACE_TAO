/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    exception.h
 *
 *  Visitor for Exceptions.
 *  This is the generic (common) visitor for all exceptions.
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
  explicit be_visitor_exception (be_visitor_context *ctx);
  ~be_visitor_exception () override = default;
  be_visitor_exception (be_visitor_exception const &) = delete;
  be_visitor_exception (be_visitor_exception &&) = delete;
  be_visitor_exception &operator= (be_visitor_exception const &) = delete;
  be_visitor_exception &operator= (be_visitor_exception &&) = delete;

  int visit_exception (be_exception *node) override;

  int visit_field (be_field *node) override;
};

#endif /* _BE_VISITOR_EXCEPTION_EXCEPTION_H_ */
