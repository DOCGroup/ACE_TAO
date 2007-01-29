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
//    serializer_op_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Structure class
//    This one provides code generation for the TAO::DCPS::Serializer operators
//    for the structure in the client header.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_STRUCTURE_serializer_op_CH_H_
#define _BE_VISITOR_STRUCTURE_serializer_op_CH_H_

class be_visitor_structure_serializer_op_ch : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_serializer_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for structure that generates the Serializer operator
  //   declarations
  //

public:
  be_visitor_structure_serializer_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_structure_serializer_op_ch (void);
  // destructor

  virtual int visit_structure (be_structure *node);
  // visit structure
};

#endif /* _BE_VISITOR_STRUCTURE_serializer_op_CH_H_ */
