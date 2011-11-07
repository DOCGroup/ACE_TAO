/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    sequence_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Sequence class
 *  This one provides code generation for the Sequence node in the client
 *  header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_SEQUENCE_SEQUENCE_CH_H_
#define _BE_VISITOR_SEQUENCE_SEQUENCE_CH_H_

/**
 * @class be_visitor_sequence_ch
 *
 * @brief be_visitor_sequence_ch
 *
 * This is a concrete visitor to generate the client header for
 * sequences
 */
class be_visitor_sequence_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_sequence_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_sequence_ch (void);

  /// visit sequence node.
  virtual int visit_sequence (be_sequence *node);

  /// Generate the typedefs for our _var and _out template classes.
  void gen_varout_typedefs (be_sequence *node,
                            be_type *elem);
};

#endif /* _BE_VISITOR_SEQUENCE_SEQUENCE_CH_H_ */
