
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
//    typedef_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Typedef class
//    This provides the code generation for the Typedef class in the client
//    stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_TYPEDEF_TYPEDEF_CS_H_
#define _BE_VISITOR_TYPEDEF_TYPEDEF_CS_H_

class be_visitor_typedef_cs : public be_visitor_typedef
{
  //
  // = TITLE
  //   be_visitor_typedef_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for typedef
  //
  //
public:
  be_visitor_typedef_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef_cs (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // visit typedef. We provide code for this method in the derived class

};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_CS_H_*/
