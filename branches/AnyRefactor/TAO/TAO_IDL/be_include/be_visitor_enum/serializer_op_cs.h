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
//    serializer_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for Enums generating code for the 
//    TAO::DCPS::Serializer operators
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ENUM_serializer_op_CS_H_
#define _BE_VISITOR_ENUM_serializer_op_CS_H_

class be_visitor_enum_serializer_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_enum_serializer_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for enum that generates the Serializer operator
  //   implementations
  //

public:
  be_visitor_enum_serializer_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_enum_serializer_op_cs (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit enum
};

#endif /* _BE_VISITOR_ENUM_serializer_op_CS_H_ */
