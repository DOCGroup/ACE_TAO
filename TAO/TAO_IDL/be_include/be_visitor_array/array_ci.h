
//=============================================================================
/**
 *  @file    array_ci.h
 *
 *  Visitors for generation of code for Arrays in client inline.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VISITOR_ARRAY_CI_H_
#define _BE_VISITOR_ARRAY_CI_H_

/**
 * @class be_visitor_array_ci
 *
 * @brief be_visitor_array_ci
 *
 * Code to be generated for arrays in the client inline file
 */
class be_visitor_array_ci : public be_visitor_array
{
public:
  /// constructor
  explicit be_visitor_array_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_array_ci () override = default;
  be_visitor_array_ci (be_visitor_array_ci const &) = delete;
  be_visitor_array_ci (be_visitor_array_ci &&) = delete;
  be_visitor_array_ci &operator= (be_visitor_array_ci const &) = delete;
  be_visitor_array_ci &operator= (be_visitor_array_ci &&) = delete;

  /// visit the array node
  int visit_array (be_array *node) override;
};

#endif /* _BE_VISITOR_ARRAY_CI_H_ */
