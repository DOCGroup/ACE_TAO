/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_fwd_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component Forward node.
 *  This one provides code generation for component forward node.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_COMPONENT_FWD_CH_H_
#define _BE_COMPONENT_COMPONENT_FWD_CH_H_

/**
 * @class be_visitor_component_fwd_ch
 *
 * @brief be_visitor_component_fwd_ch
 *
 * This is the visitor for component_fwd for the header file
 */
class be_visitor_component_fwd_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_component_fwd_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_component_fwd_ch (void);

  /// visit interface_fwd.
  virtual int visit_component_fwd (be_component_fwd *node);
};

#endif /* _BE_COMPONENT_COMPONENT_FWD_CH_H_ */
