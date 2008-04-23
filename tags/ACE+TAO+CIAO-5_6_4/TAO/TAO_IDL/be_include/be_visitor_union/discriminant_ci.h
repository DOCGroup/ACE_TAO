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
//    discriminant_ci.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code for the discriminant of the Union in the client
//     inline file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_DISCRIMINANT_CI_H_
#define _BE_VISITOR_UNION_DISCRIMINANT_CI_H_

class be_visitor_union_discriminant_ci : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_union_discriminant_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for union
  //   discriminant
  //
  //
public:
  be_visitor_union_discriminant_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_discriminant_ci (void);
  // destructor

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type

  virtual int visit_typedef (be_typedef *node);
  // visit a typedef node
};

#endif /* _BE_VISITOR_UNION_DISCRIMINANT_CI_H_ */
