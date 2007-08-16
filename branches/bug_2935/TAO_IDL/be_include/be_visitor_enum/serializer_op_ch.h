/* -*- C++ -*- */
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
//    Concrete visitor for Enums generating code for the 
//    TAO::DCPS::Serializer operators
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ENUM_serializer_op_CH_H_
#define _BE_VISITOR_ENUM_serializer_op_CH_H_

class be_visitor_enum_serializer_op_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_enum_serializer_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for enum that generates the Serializer operator
  //   declarations
  //

public:
  be_visitor_enum_serializer_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_enum_serializer_op_ch (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit enum
};

#endif /* _BE_VISITOR_ENUM_serializer_op_CH_H_ */
