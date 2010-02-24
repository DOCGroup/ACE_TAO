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
//    serializer_op.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for the TAO::DCPS::Serializer operators
//    for elements of the Root scope.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_serializer_op_H_
#define _BE_VISITOR_ROOT_serializer_op_H_

class be_visitor_root_serializer_op : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_serializer_op
  //
  // = DESCRIPTION
  //   This is a concrete visitor for root that generates the Serializer operator
  //   declarations and implementations
  //

public:
  be_visitor_root_serializer_op (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_serializer_op (void);
  // destructor

  virtual int visit_root (be_root *node);
  // visit root
};

#endif /* _BE_VISITOR_ROOT_serializer_op_H_ */
