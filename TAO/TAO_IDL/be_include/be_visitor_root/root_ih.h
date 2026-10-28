
//=============================================================================
/**
 *  @file    root_ih.h
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  implementation header.
 *
 *  @author Yamuna Krishnamurthy
 */
//=============================================================================

#ifndef _BE_VISITOR_ROOT_ROOT_IH_H_
#define _BE_VISITOR_ROOT_ROOT_IH_H_

/**
 * @class be_visitor_root_ih
 *
 * @brief be_visitor_root_ih
 *
 * This is a concrete visitor to generate the implementation header for root
 */
class be_visitor_root_ih : public be_visitor_root
{
public:
  explicit be_visitor_root_ih (be_visitor_context *ctx);

  ~be_visitor_root_ih () override = default;

  be_visitor_root_ih (const be_visitor_root_ih &) = delete;
  be_visitor_root_ih (be_visitor_root_ih &&) = delete;
  be_visitor_root_ih &operator= (const be_visitor_root_ih &) = delete;
  be_visitor_root_ih &operator= (be_visitor_root_ih &&) = delete;

  int visit_root (be_root *node) override;

private:
  /// Open file and initialize stream.
  int init ();
};

#endif /* _BE_VISITOR_ROOT_ROOT_IH_H_ */
