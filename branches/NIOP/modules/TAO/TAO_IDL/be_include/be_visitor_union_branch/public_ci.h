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
//    public_ci.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code for public part of the union class for the union
//     members in the client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_BRANCI_PUBLIC_CI_H_
#define _BE_VISITOR_UNION_BRANCI_PUBLIC_CI_H_

class be_visitor_union_branch_public_ci : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_public_ci
  //
  // = DESCRIPTION
  //    This visitor is used to generate the accessors for the members of the
  //    union
  //
public:
  be_visitor_union_branch_public_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_public_ci (void);
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

  virtual int visit_valuebox (be_valuebox *node);
  // visit valuebox type

  virtual int visit_valuetype (be_valuetype *node);
  // visit value type

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit value forward type

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

private:
  int emit_valuetype_common  (be_type *node);

};

#endif /* _BE_VISITOR_UNION_BRANCI_PUBLIC_CI_H_ */
