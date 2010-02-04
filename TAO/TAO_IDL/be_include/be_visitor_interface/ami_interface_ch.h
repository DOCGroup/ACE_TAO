/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    interface_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the AMI Interface node.
 *  This one provides code generation for interfaces in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_INTERFACE_AMI_INTERFACE_CH_H_
#define _BE_INTERFACE_AMI_INTERFACE_CH_H_

// we need derived interface visitors for the client and server header files. For
// the others, they use the default interface visitor

/**
 * @class be_visitor_ami_interface_ch
 *
 * @brief be_visitor_interface_ch
 *
 * This is a concrete visitor to generate the client header for interface
 */
class be_visitor_ami_interface_ch : public be_visitor_interface
{
public:
  /// constructor
  be_visitor_ami_interface_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_ami_interface_ch (void);

  /// set the right context and make a visitor
  virtual int visit_interface (be_interface *node);
};

#endif /* _BE_INTERFACE_AMI_INTERFACE_CH_H_ */
