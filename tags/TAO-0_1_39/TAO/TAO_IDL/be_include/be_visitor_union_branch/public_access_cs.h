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
//    public_access_cs.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code for the access method of the union class
//     for the union members. This is primarily used by the marshaling engine.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (_BE_VISITOR_UNION_BRANCH_PUBLIC_ACCESS_CS_H_)
#define _BE_VISITOR_UNION_BRANCH_PUBLIC_ACCESS_CS_H_

class be_visitor_union_branch_public_access_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_public_access_cs
  //
  // = DESCRIPTION
  //    This is used to generate the body of the access method
  //
public:
  be_visitor_union_branch_public_access_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_public_access_cs (void);
  // destructor

  virtual int visit_union_branch (be_union_branch *node);
  // visit the union_branch node

  // =visit operations on all possible data types that a union_branch can be

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_enum (be_enum *node);
  // visit enum type

  virtual int visit_interface (be_interface *node);
  // visit interface type

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit interface forward type

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_string (be_string *node);
  // visit string type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type

};

#endif /* _BE_VISITOR_UNION_BRANCH_PUBLIC_ACCESS_CS_H_ */
