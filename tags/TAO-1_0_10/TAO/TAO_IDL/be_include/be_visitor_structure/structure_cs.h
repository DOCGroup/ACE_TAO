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
//    structure_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Structure class
//    This one provides code generation for the structure in the client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_STRUCTURE_STRUCTURE_CS_H_
#define _BE_VISITOR_STRUCTURE_STRUCTURE_CS_H_

class be_visitor_structure_cs : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for structure
  //
  //
public:
  be_visitor_structure_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_structure_cs (void);
  // destructor

  virtual int visit_structure (be_structure *node);
  // visit structure.

};

#endif /* _BE_VISITOR_STRUCTURE_STRUCTURE_CS_H_ */
