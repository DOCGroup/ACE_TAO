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
//    typedef_ci.h
//
// = DESCRIPTION
//    Concrete visitor for the Typedef class
//    This provides the code generation for the Typedef class in the client
//    inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_TYPEDEF_TYPEDEF_CI_H_
#define _BE_VISITOR_TYPEDEF_TYPEDEF_CI_H_

class be_visitor_typedef_ci : public be_visitor_typedef
{
  //
  // = TITLE
  //   be_visitor_typedef_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for typedef
  //
  //
public:
  be_visitor_typedef_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef_ci (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // visit typedef. We provide code for this method in the derived class

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union
};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_CI_H_*/
