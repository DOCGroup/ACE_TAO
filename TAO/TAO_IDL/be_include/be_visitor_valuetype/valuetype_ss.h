/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_ss.h
 *
 *  Concrete visitor for the valuetype node.
 *  This provides for code generation in the server skeleton
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_SS_H_
#define _BE_VALUETYPE_VALUETYPE_SS_H_

#include <string>

/**
 * @class be_visitor_valuetype_ss
 *
 * @brief be_visitor_valuetype_ss
 *
 * This is a concrete visitor to generate the server skeletons for
 * interface
 */
class be_visitor_valuetype_ss : public be_visitor_valuetype
{
public:
  /// constructor
  explicit be_visitor_valuetype_ss (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_ss () override;
  be_visitor_valuetype_ss (be_visitor_valuetype_ss const &) = delete;
  be_visitor_valuetype_ss (be_visitor_valuetype_ss &&) = delete;
  be_visitor_valuetype_ss &operator= (be_visitor_valuetype_ss const &) = delete;
  be_visitor_valuetype_ss &operator= (be_visitor_valuetype_ss &&) = delete;

  int visit_valuetype (be_valuetype *node) override;
  int visit_eventtype (be_eventtype *node) override;

protected:
  std::string_view generate_local_name (be_valuetype *node);
  std::string_view generate_full_skel_name (be_valuetype *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_SS_H_ */
