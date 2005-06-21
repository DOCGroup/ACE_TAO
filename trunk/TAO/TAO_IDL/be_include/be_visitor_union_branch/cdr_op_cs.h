//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the base "BE_Union_Branch" node
//    This generates code for structure members in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_BRANCH_CDR_OP_CS_H_
#define _BE_VISITOR_UNION_BRANCH_CDR_OP_CS_H_

class be_visitor_union_branch_cdr_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_cdr_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for the be_union_branch node for the client header.
  //
public:
  be_visitor_union_branch_cdr_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_cdr_op_cs (void);
  // destructor

  virtual int visit_union_branch (be_union_branch *node);
  // visit the union_branch node

  // =visit operations on all possible data types that a union_branch can be

  virtual int visit_array (be_array *node);
  virtual int visit_enum (be_enum *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_interface_fwd (be_interface_fwd *node);
  virtual int visit_valuebox (be_valuebox *node);
  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_predefined_type (be_predefined_type *node);
  virtual int visit_sequence (be_sequence *node);
  virtual int visit_string (be_string *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_union (be_union *node);

  virtual int explicit_default (void);
  // extra code generation for an explicit default case

 private:
  int emit_valuetype_common (be_type *node);
};

#endif /*  _BE_VISITOR_UNION_BRANCH_CDR_OP_CS_H_ */
