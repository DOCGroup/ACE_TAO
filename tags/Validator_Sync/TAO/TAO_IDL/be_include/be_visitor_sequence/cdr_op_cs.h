//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.h
//
// = DESCRIPTION
//    Visitors for generation of code for Sequences in client stubs. This one
//    generates the CDR operators.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_SEQUENCE_CDR_OP_CS_H_
#define _BE_VISITOR_SEQUENCE_CDR_OP_CS_H_

// ************************************************************
// class be_visitor_sequence_cs
// ************************************************************

class be_visitor_sequence_cdr_op_cs : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_sequence_cdr_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for sequence that generates the CDR operator
  //   implementations
  //

public:
  be_visitor_sequence_cdr_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_sequence_cdr_op_cs (void);
  // destructor

  virtual int visit_sequence (be_sequence *node);
  // visit sequence

  // = Visitor methods on sequence types

  virtual int visit_array (be_array *node);
  // visit sequence

  virtual int visit_enum (be_enum *node);
  // visit an enum

  virtual int visit_interface (be_interface *node);
  // visit an interface

  virtual int visit_interface_fwd (be_interface_fwd *node);
  // visit an interface forward node

  virtual int visit_valuetype (be_valuetype *node);
  // visit an interface

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  // visit an interface forward node

  virtual int visit_predefined_type (be_predefined_type *node);
  // visit a predefined type node

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

#endif /* _BE_VISITOR_SEQUENCE_CDR_OP_CS_H_ */
