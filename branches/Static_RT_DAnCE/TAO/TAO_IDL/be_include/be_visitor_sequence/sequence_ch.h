/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    sequence_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Sequence class
//    This one provides code generation for the Sequence node in the client
//    header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_SEQUENCE_SEQUENCE_CH_H_
#define _BE_VISITOR_SEQUENCE_SEQUENCE_CH_H_

class be_visitor_sequence_ch : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_sequence_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for
  //   sequences
  //
public:
  be_visitor_sequence_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_ch (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence node.

  void gen_varout_typedefs (be_sequence *node, 
                            be_type *elem);
  // Generate the typedefs for our _var and _out template classes.
};

#endif /* _BE_VISITOR_SEQUENCE_SEQUENCE_CH_H_ */
