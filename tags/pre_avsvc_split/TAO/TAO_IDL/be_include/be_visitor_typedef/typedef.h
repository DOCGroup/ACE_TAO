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
//    typedef.h
//
// = DESCRIPTION
//    Concrete visitor for the Typedef class
//    This provides the generic visitors for the Typedef class.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_TYPEDEF_TYPEDEF_H_
#define _BE_VISITOR_TYPEDEF_TYPEDEF_H_

class be_visitor_typedef : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_typedef
  //
  // = DESCRIPTION
  //    This is a concrete visitor for typedef that abstracts all common tasks
  //

public:
  be_visitor_typedef (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // must be overridden

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type node

  virtual int visit_string (be_string *node);
  // visit a typedef

  virtual int visit_sequence (be_sequence *node);
  // visit a sequence

  virtual int visit_structure (be_structure *node);
  // visit a structure

  virtual int visit_union (be_union *node);
  // visit a union

};

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_H_ */
