/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    smart_proxy_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This provides code generation for smart proxy classes for an
 *  interface in the client header.
 *
 *
 *  @author Kirthika Parameswaran
 */
//=============================================================================


#ifndef _BE_INTERFACE_SMART_PROXY_CH_H_
#define _BE_INTERFACE_SMART_PROXY_CH_H_

/**
 * @class be_visitor_interface_smart_proxy_ch
 *
 * @brief Generate the "smart proxy" class declaration.
 */
class be_visitor_interface_smart_proxy_ch : public be_visitor_interface
{

public:
  /// constructor
  be_visitor_interface_smart_proxy_ch (be_visitor_context *ctx);

  /// destructor
  virtual ~be_visitor_interface_smart_proxy_ch (void);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// This will just call the above method - no need to create
  /// another set of visitors for this stuff.
  virtual int visit_component (be_component *node);
};

#endif /* _BE_INTERFACE_SMART_PROXY_CH_H_ */
