/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    structure_fwd_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the be_structure_fwd node.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_INTERFACE_STRUCTURE_FWD_CH_H_
#define _BE_INTERFACE_STRUCTURE_FWD_CH_H_

/**
 * @class be_visitor_structure_fwd_ch
 *
 * @brief be_visitor_structure_fwd_ch
 *
 * This is the visitor for be_structure_fwd for the header file
 */
class be_visitor_structure_fwd_ch : public be_visitor_decl
{
public:
  be_visitor_structure_fwd_ch (be_visitor_context *ctx);

  ~be_visitor_structure_fwd_ch (void);

  virtual int visit_structure_fwd (be_structure_fwd *node);
};

#endif /* _BE_INTERFACE_STRUCTURE_FWD_CH_H_ */
