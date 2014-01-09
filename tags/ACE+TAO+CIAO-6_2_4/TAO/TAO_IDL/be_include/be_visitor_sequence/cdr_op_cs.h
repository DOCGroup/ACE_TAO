
//=============================================================================
/**
 *  @file    cdr_op_cs.h
 *
 *  $Id$
 *
 *  Visitors for generation of code for Sequences in client stubs. This one
 *  generates the CDR operators.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_SEQUENCE_CDR_OP_CS_H_
#define _BE_VISITOR_SEQUENCE_CDR_OP_CS_H_

// ************************************************************
// class be_visitor_sequence_cs
// ************************************************************

/**
 * @class be_visitor_sequence_cdr_op_cs
 *
 * @brief be_visitor_sequence_cdr_op_cs
 *
 * This is a concrete visitor for sequence that generates the CDR operator
 * implementations
 */
class be_visitor_sequence_cdr_op_cs : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_sequence_cdr_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_sequence_cdr_op_cs (void);

  /// visit sequence
  virtual int visit_sequence (be_sequence *node);

  // = Visitor methods on sequence types

  /// visit sequence
  virtual int visit_array (be_array *node);

  /// visit an enum
  virtual int visit_enum (be_enum *node);

  /// visit an interface
  virtual int visit_interface (be_interface *node);

  /// visit an interface forward node
  virtual int visit_interface_fwd (be_interface_fwd *node);

  /// visit a component
  virtual int visit_component (be_component *node);

  /// visit a component forward node
  virtual int visit_component_fwd (be_component_fwd *node);

  /// visit a home
  virtual int visit_home (be_home *node);

  /// visit valuebox
  virtual int visit_valuebox (be_valuebox *node);

  /// visit a valuetype
  virtual int visit_valuetype (be_valuetype *node);

  /// visit a valuetype forward node
  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);

  /// visit an eventtype
  virtual int visit_eventtype (be_eventtype *node);

  /// visit an eventtype forward node
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);

  /// visit a predefined type node
  virtual int visit_predefined_type (be_predefined_type *node);

  /// visit string
  virtual int visit_string (be_string *node);

  /// visit structure
  virtual int visit_structure (be_structure *node);

  /// visit typedef
  virtual int visit_typedef (be_typedef *node);

  /// visit union
  virtual int visit_union (be_union *node);

protected:

  /// helper that does the common job
  int visit_node (be_type *);

};

#endif /* _BE_VISITOR_SEQUENCE_CDR_OP_CS_H_ */
