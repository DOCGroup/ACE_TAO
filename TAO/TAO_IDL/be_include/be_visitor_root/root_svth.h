/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_svth.h
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  CIAO servant header.
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
  explicit be_visitor_root_svth (be_visitor_context *ctx);

  ~be_visitor_root_svth () override;

  be_visitor_root_svth (const be_visitor_root_svth &) = delete;
  be_visitor_root_svth (be_visitor_root_svth &&) = delete;
  be_visitor_root_svth &operator= (const be_visitor_root_svth &) = delete;
  be_visitor_root_svth &operator= (be_visitor_root_svth &&) = delete;

  int visit_root (be_root *node) override;

private:
  /// Open file and initialize stream.
  int init ();
};

#endif /* _BE_VISITOR_ROOT_ROOT_SVTH_H_ */
