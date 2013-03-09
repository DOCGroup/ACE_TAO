/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    structure_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Structure class
 *  This one provides code generation for the structure in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_STRUCTURE_STRUCTURE_CH_H_
#define _BE_VISITOR_STRUCTURE_STRUCTURE_CH_H_

/**
 * @class be_visitor_structure_ch
 *
 * @brief be_visitor_structure_ch
 *
 * This is a concrete visitor to generate the client header for structure
 */
class be_visitor_structure_ch : public be_visitor_structure
{
public:
  /// constructureor
  be_visitor_structure_ch (be_visitor_context *ctx);

  /// destructureor
  ~be_visitor_structure_ch (void);

  /// visit structure. We provide code for this method in the derived class
  virtual int visit_structure (be_structure *node);
};

#endif /* _BE_VISITOR_STRUCTURE_STRUCTURE_CH_H_ */
