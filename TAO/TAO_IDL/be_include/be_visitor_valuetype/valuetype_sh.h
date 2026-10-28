/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_sh.h
 *
 *  Concrete visitor for the valuetype node.
 *  This provides for code generation in the server header
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_SH_H_
#define _BE_VALUETYPE_VALUETYPE_SH_H_

/**
 * @class be_visitor_valuetype_sh
 *
 * @brief be_visitor_valuetype_sh
 *
 * This is a concrete visitor to generate the server header for valuetype
 */
class be_visitor_valuetype_sh : public be_visitor_valuetype
{
public:
  /// constructor
  explicit be_visitor_valuetype_sh (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_sh () override = default;
  be_visitor_valuetype_sh (be_visitor_valuetype_sh const &) = delete;
  be_visitor_valuetype_sh (be_visitor_valuetype_sh &&) = delete;
  be_visitor_valuetype_sh &operator= (be_visitor_valuetype_sh const &) = delete;
  be_visitor_valuetype_sh &operator= (be_visitor_valuetype_sh &&) = delete;

  int visit_valuetype (be_valuetype *node) override;
  int visit_eventtype (be_eventtype *node) override;
};

#endif /* _BE_VALUETYPE_VALUETYPE_SH_H_ */
