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
//    enum_cs.h
//
// = DESCRIPTION
//    Concrete visitor for Enums generating code in the client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ENUM_ENUM_CS_H_
#define _BE_VISITOR_ENUM_ENUM_CS_H_

class be_visitor_enum_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_enum_cs
  //
  // = DESCRIPTION
  //   This is the visitor for enum for the impl file
  //
  //
public:
  be_visitor_enum_cs (be_visitor_context *ctx);
  // conenumor

  ~be_visitor_enum_cs (void);
  // deenumor

  virtual int visit_enum (be_enum *node);
  // visit enum and generate the typecode

};

#endif /* _BE_VISITOR_ENUM_ENUM_CS_H_ */
