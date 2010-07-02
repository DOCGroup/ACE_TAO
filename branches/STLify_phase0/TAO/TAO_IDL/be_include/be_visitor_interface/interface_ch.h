/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    interface_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This one provides code generation for interfaces in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_INTERFACE_INTERFACE_CH_H_
#define _BE_INTERFACE_INTERFACE_CH_H_

// we need derived interface visitors for the client and server header files. For
// the others, they use the default interface visitor

/**
 * @class be_visitor_interface_ch
 *
 * @brief be_visitor_interface_ch
 *
 * This is a concrete visitor to generate the client header for interface
 */
class be_visitor_interface_ch : public be_visitor_interface
{
public:
  /// constructor
  be_visitor_interface_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_interface_ch (void);

  /// set the right context and make a visitor
  virtual int visit_interface (be_interface *node);

  /// Helper to generate the declarations for the operations
  /// of any abstract parents we may have.
  static int gen_abstract_ops_helper (be_interface *node,
                                      be_interface *base,
                                      TAO_OutStream *os);
private:
  /// Helper to generate declarations for _narrow () and
  /// _unchecked_narrow ()
  bool gen_xxx_narrow (const char *n,
                       be_interface *node,
                       TAO_OutStream *os);
};

#endif /* _BE_INTERFACE_INTERFACE_CH_H_ */
