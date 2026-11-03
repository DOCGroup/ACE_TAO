/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_ss.h
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  server skeletons.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_SS_H_
#define _BE_VISITOR_ROOT_ROOT_SS_H_

/**
 * @class be_visitor_root_ss
 *
 * @brief be_visitor_root_ss
 *
 * This is a concrete visitor to generate the server skeletons for root
 */
class be_visitor_root_ss : public be_visitor_root
{
public:
  explicit be_visitor_root_ss (be_visitor_context *ctx);

  ~be_visitor_root_ss () override;

  be_visitor_root_ss (const be_visitor_root_ss &) = delete;
  be_visitor_root_ss (be_visitor_root_ss &&) = delete;
  be_visitor_root_ss &operator= (const be_visitor_root_ss &) = delete;
  be_visitor_root_ss &operator= (be_visitor_root_ss &&) = delete;

  int visit_root (be_root *node) override;

private:
  /// Open file and initialize stream.
  int init ();
};

#endif /* _BE_VISITOR_ROOT_ROOT_SS_H_ */
