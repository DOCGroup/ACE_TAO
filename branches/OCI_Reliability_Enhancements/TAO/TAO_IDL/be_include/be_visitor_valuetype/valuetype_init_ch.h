/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_init_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Valuetype node.
 *  This one provides code generation for valuetype factory
 *  in the (client) header.
 *
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
  be_visitor_valuetype_init_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_init_ch (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);

  /// visit a factory
  virtual int visit_factory (be_factory *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_INIT_CH_H_ */
