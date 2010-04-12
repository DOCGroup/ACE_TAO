/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    amh_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the AMH-RH Interface node.
 *  This one provides code generation for interfaces in the client
 *  header.
 *
 *
 *  @author Mayur Deshpande
 */
//=============================================================================


#ifndef _BE_INTERFACE_AMH_INTERFACE_CH_H_
#define _BE_INTERFACE_AMH_INTERFACE_CH_H_

/**
 * @class be_visitor_amh_interface_ch
 *
 * @brief be_visitor_amh_interface_ch
 *
 * This is a concrete visitor to generate the client header for
 * AMH interface
 */
class be_visitor_amh_interface_ch : public be_visitor_interface
{
public:
  /// constructor
  be_visitor_amh_interface_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_amh_interface_ch (void);

  /// set the right context and make a visitor
  virtual int visit_interface (be_interface *node);
};

#endif /* _BE_INTERFACE_AMH_INTERFACE_CH_H_ */
