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
//    for the structure in the client stub.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_STRUCTURE_serializer_op_CS_H_
#define _BE_VISITOR_STRUCTURE_serializer_op_CS_H_

class be_visitor_structure_serializer_op_cs : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_serializer_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for structure that generates the Serializer operator
  //   implementations
  //

public:
  be_visitor_structure_serializer_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_structure_serializer_op_cs (void);
  // destructor

  virtual int visit_structure (be_structure *node);
  // visit structure

  virtual int post_process (be_decl *);
  // any post processing that needs to be done after a field is handled

  virtual int pre_process (be_decl *bd);
  // any preprocessing that needs to be done before a field is handled
};

#endif /* _BE_VISITOR_STRUCTURE_serializer_op_CS_H_ */
