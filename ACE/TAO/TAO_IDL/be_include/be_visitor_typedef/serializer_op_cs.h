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
//    serializer_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Typedef class
//    This provides the code generation for the TAO::DCPS::Serializer operators
//    of the Typedef class in the client stubs.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_TYPEDEF_serializer_op_CS_H_
#define _BE_VISITOR_TYPEDEF_serializer_op_CS_H_

class be_visitor_typedef_serializer_op_cs : public be_visitor_typedef
{
  //
  // = TITLE
  //   be_visitor_typedef_serializer_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for typedef that generates the Serializer operator
  //   declarations and implementations
  //

public:
  be_visitor_typedef_serializer_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef_serializer_op_cs (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // visit typedef

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union
};

#endif /* _BE_VISITOR_TYPEDEF_serializer_op_CS_H_*/
