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
//    union_cs.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code in the client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_UNION_CS_H_
#define _BE_VISITOR_UNION_UNION_CS_H_

class be_visitor_union_cs : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for union
  //
  //
public:
  be_visitor_union_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_cs (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union. We provide code for this method in the derived class

};

#endif /* _BE_VISITOR_UNION_UNION_CS_H_ */
