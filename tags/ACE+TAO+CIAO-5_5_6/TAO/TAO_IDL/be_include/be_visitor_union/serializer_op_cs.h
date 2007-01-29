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
//    Concrete visitor for the Union class
//    This one provides code generation for the TAO::DCPS::Serializer operators
//    for the union in the client stub.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_serializer_op_CS_H_
#define _BE_VISITOR_UNION_serializer_op_CS_H_

class be_visitor_union_serializer_op_cs : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_serializer_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for union that generates the Serializer operator
  //   implementations
  //

public:
  be_visitor_union_serializer_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_serializer_op_cs (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union

  virtual int pre_process (be_decl *);
  virtual int post_process (be_decl *);
};

#endif /* _BE_VISITOR_UNION_serializer_op_CS_H_ */
