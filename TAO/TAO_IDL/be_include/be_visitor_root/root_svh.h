/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_svh.h
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO servant header.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SVH_H_
#define _BE_VISITOR_ROOT_ROOT_SVH_H_

/**
 * @class be_visitor_root_svh
 *
 * @brief be_visitor_root_svh
 *
 * This is a concrete visitor to generate the CIAO servant
 * header for root
 */
class be_visitor_root_svh : public be_visitor_root
{
public:
  explicit be_visitor_root_svh (be_visitor_context *ctx);

  ~be_visitor_root_svh () override = default;

  be_visitor_root_svh (const be_visitor_root_svh &) = delete;
  be_visitor_root_svh (be_visitor_root_svh &&) = delete;
  be_visitor_root_svh &operator= (const be_visitor_root_svh &) = delete;
  be_visitor_root_svh &operator= (be_visitor_root_svh &&) = delete;

  int visit_root (be_root *node) override;

private:
  /// Open file and initialize stream.
  int init ();
};

#endif /* _BE_VISITOR_ROOT_ROOT_SVH_H_ */
