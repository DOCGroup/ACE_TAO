/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    tie_sh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This one provides the code generation for TIE classes
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_INTERFACE_TIE_SH_H_
#define _BE_INTERFACE_TIE_SH_H_

/**
 * @class be_visitor_interface_tie_sh
 *
 * @brief be_visitor_interface_tie_sh
 *
 * This is a concrete visitor to generate the server header for the TIE class
 */
class be_visitor_interface_tie_sh : public be_visitor_interface
{
public:
  /// constructor
  be_visitor_interface_tie_sh (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_interface_tie_sh (void);

  /// set the right context and make a visitor
  virtual int visit_interface (be_interface *node);

  /// set the right context and make a visitor
  virtual int visit_component (be_component *node);

  static int method_helper (be_interface *,
                            be_interface *,
                            TAO_OutStream *os);
};

#endif /* _BE_INTERFACE_TIE_SH_H_ */
