/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    interface_fwd_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface Forward node.
 *  This one provides code generation for interface forward node.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_INTERFACE_INTERFACE_FWD_CH_H_
#define _BE_INTERFACE_INTERFACE_FWD_CH_H_

/**
 * @class be_visitor_interface_fwd_ch
 *
 * @brief be_visitor_interface_fwd_ch
 *
 * This is the visitor for interface_fwd for the header file
 */
class be_visitor_interface_fwd_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_interface_fwd_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_interface_fwd_ch (void);

  /// visit interface_fwd.
  virtual int visit_interface_fwd (be_interface_fwd *node);
};

#endif /* _BE_INTERFACE_INTERFACE_FWD_CH_H_ */
