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
//    private_ch.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code for private part of the union class for the union
//     members in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_BRANCH_PRIVATE_CH_H_
#define _BE_VISITOR_UNION_BRANCH_PRIVATE_CH_H_

class be_visitor_union_branch_private_ch : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_private_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_union_branch node for the client
  //   header. This generates the code for the private section of the "union"
  //   class
  //
public:
  be_visitor_union_branch_private_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_private_ch (void);
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

  virtual int visit_valuetype (be_valuetype *node);
  // visit interface type

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
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

#endif /* _BE_VISITOR_UNION_BRANCH_PRIVATE_CH_H_ */
