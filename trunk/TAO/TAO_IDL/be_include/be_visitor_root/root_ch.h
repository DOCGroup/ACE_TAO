/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    root_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Root class
 *  This one provides code generation for elements of the Root node in the
 *  client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ROOT_ROOT_CH_H_
#define _BE_VISITOR_ROOT_ROOT_CH_H_

/**
 * @class be_visitor_root_ch
 *
 * @brief be_visitor_root_ch
 *
 * This is a concrete visitor to generate the client header for root
 */
class be_visitor_root_ch : public be_visitor_root
{
public:
  /// constructor
  be_visitor_root_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_root_ch (void);

  /// set the right context and make a visitor
  virtual int init (void);
};

#endif /* _BE_VISITOR_ROOT_ROOT_CH_H_ */
