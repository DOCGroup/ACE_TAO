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
//    discriminant_cs.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code for the discriminant of the Union in the client
//     stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_DISCRIMINANT_CS_H_
#define _BE_VISITOR_UNION_DISCRIMINANT_CS_H_

class be_visitor_union_discriminant_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_union_discriminant_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for union
  //   discriminant
  //
  //
public:
  be_visitor_union_discriminant_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_discriminant_cs (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit an enum. Required to generate the typecode for an enum definition
  // appearing side the union

};

#endif /* _BE_VISITOR_UNION_DISCRIMINANT_CS_H_ */
