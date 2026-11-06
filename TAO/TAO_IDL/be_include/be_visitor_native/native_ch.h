/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    native_ch.h
 *
 *  Concrete visitor for Native generating code in the client header.
 *
 *  @author Johnny Willemsen
 */
//=============================================================================

#ifndef _BE_VISITOR_NATIVE_NATIVE_CH_H_
#define _BE_VISITOR_NATIVE_NATIVE_CH_H_

/**
 * @class be_visitor_native_ch
 *
 * @brief be_visitor_native_ch
 *
 * This is the base visitor for native for the header file
 */
class be_visitor_native_ch : public be_visitor_scope
{
public:
  /// constructor
  explicit be_visitor_native_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_native_ch () override = default;

  be_visitor_native_ch (const be_visitor_native_ch &) = delete;
  be_visitor_native_ch (be_visitor_native_ch &&) = delete;
  be_visitor_native_ch &operator= (const be_visitor_native_ch &) = delete;
  be_visitor_native_ch &operator= (be_visitor_native_ch &&) = delete;

  /// visit native.
  int visit_native (be_native *node) override;
};

#endif /* _BE_VISITOR_NATIVE_NATIVE_CH_H_ */
