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
//    typedef_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Typedef class
//    This provides the code generation for the Typedef class in the client
//    header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_TYPEDEF_TYPEDEF_CH_H_
#define _BE_VISITOR_TYPEDEF_TYPEDEF_CH_H_

class be_visitor_typedef_ch : public be_visitor_typedef
{
  //
  // = TITLE
  //   be_visitor_typedef_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for typedef
  //
  //
public:
  be_visitor_typedef_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_typedef_ch (void);
  // destructor

  virtual int visit_typedef (be_typedef *node);
  // visit typedef. We provide code for this method in the derived class

  // =visit methods on all elements syntactically valid as types that can be
  // typedefed.

  virtual int visit_array (be_array *node);
  // visit a array

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_interface (be_interface *node);
  // visit an interface;

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

#endif /* _BE_VISITOR_TYPEDEF_TYPEDEF_CH_H_ */
