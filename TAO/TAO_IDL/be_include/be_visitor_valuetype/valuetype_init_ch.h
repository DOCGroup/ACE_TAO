/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_init_ch.h
 *
 *  Concrete visitor for the Valuetype node.
 *  This one provides code generation for valuetype factory
 *  in the (client) header.
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_INIT_CH_H_
#define _BE_VALUETYPE_VALUETYPE_INIT_CH_H_


/**
 * @class be_visitor_valuetype_init_ch
 *
 * @brief be_visitor_valuetype_init_ch
 *
 * This is a concrete visitor to generate valuetype factory in
 * the client header
 */
class be_visitor_valuetype_init_ch : public be_visitor_valuetype_init
{
public:
  /// constructor
  explicit be_visitor_valuetype_init_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_init_ch () override;
  be_visitor_valuetype_init_ch (be_visitor_valuetype_init_ch const &) = delete;
  be_visitor_valuetype_init_ch (be_visitor_valuetype_init_ch &&) = delete;
  be_visitor_valuetype_init_ch &operator= (be_visitor_valuetype_init_ch const &) = delete;
  be_visitor_valuetype_init_ch &operator= (be_visitor_valuetype_init_ch &&) = delete;

  int visit_valuetype (be_valuetype *node) override;
  int visit_eventtype (be_eventtype *node) override;

  /// visit a factory
  int visit_factory (be_factory *node) override;
};

#endif /* _BE_VALUETYPE_VALUETYPE_INIT_CH_H_ */
