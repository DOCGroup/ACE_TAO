//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    array.h
//
// = DESCRIPTION
//    Visitors for generation of code for Arrays.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_ARRAY_H_
#define _BE_VISITOR_ARRAY_ARRAY_H_

// ************************************************************
// class be_visitor_array
// ************************************************************

class be_visitor_array : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_array
  //
  // = DESCRIPTION
  //   This is a generic visitor for arrays
  //
  //
public:
  be_visitor_array (be_visitor_context *ctx);
  // Constructor

  ~be_visitor_array (void);
  // destructor

  virtual int visit_array (be_array *);
  // visit the array node

  // = Visitor methods on array types

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_interface (be_interface *node);
  // visit an interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface forward node

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

  virtual int visit_sequence (be_sequence *node);
  // visit sequence

  virtual int visit_string (be_string *node);
  // visit string

  virtual int visit_structure (be_structure *node);
  // visit structure

  virtual int visit_typedef (be_typedef *node);
  // visit typedef

  virtual int visit_union (be_union *node);
  // visit union

protected:

  int visit_node (be_type *);
  // helper that does the common job

};

#endif /* _BE_VISITOR_ARRAY_ARRAY_H_*/
