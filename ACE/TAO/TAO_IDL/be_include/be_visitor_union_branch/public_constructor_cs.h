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
//    public_constructor_cs.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code for the constructor of the union class
//
// = AUTHOR
//    Johnny Willemsen
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_BRANCH_PUBLIC_CONSTRUCTOR_CS_H_
#define _BE_VISITOR_UNION_BRANCH_PUBLIC_CONSTRUCTOR_CS_H_

class be_visitor_union_branch_public_constructor_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_public_constructor_cs
  //
  // = DESCRIPTION
  //    This is used to generate the body of the constructor
  //
public:
  be_visitor_union_branch_public_constructor_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_public_constructor_cs (void);
  // destructor

  virtual int visit_union_branch (be_union_branch *node);
  // visit the union_branch node

  // =visit operations on all possible data types that a union_branch can be

  virtual int visit_array (be_array *node);
  // visit array type

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit predefined type

  virtual int visit_sequence (be_sequence *node);
  // visit sequence type

  virtual int visit_structure (be_structure *node);
  // visit structure type

  virtual int visit_typedef (be_typedef *node);
  // visit typedefed type

  virtual int visit_union (be_union *node);
  // visit union type
};

#endif /* _BE_VISITOR_UNION_BRANCH_PUBLIC_CONSTRUCTOR_CS_H_ */
